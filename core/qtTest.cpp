/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QFileInfo>
#include <QRegExp>
#include <QStack>
#include <QThreadStorage>

#include "qtStlUtil.h"
#include "qtTest.h"
#include "qtUtil.h"

namespace // anonymous
{

static void qtMessageHandler(QtMsgType, const char* msg,
                             qtTest::StreamPointer stream)
{
  static QThreadStorage<qtTest::StreamPointer*> ts;
  qtTest::StreamPointer* tsp = ts.localData();

  if (stream || msg == 0)
    {
    if (tsp)
      {
      (*tsp) = stream;
      }
    else
      {
      ts.setLocalData(new qtTest::StreamPointer(stream));
      }
    }
  else
    {
    if (tsp && *tsp)
      {
      (**tsp) << msg << '\n';
      }
    }
}

static void qtMessageHandler(QtMsgType type, const char* msg)
{
  qtMessageHandler(type, msg, qtTest::StreamPointer());
}

} // namespace <anonymous>

//-----------------------------------------------------------------------------
class qtTestPrivate
{
public:
  qtTestPrivate()
    : testResult(0), collectedResult(0), totalTests(0), totalPassed(0),
      err(new QTextStream(stderr, QIODevice::WriteOnly))
    {}

  int testResult, collectedResult;
  int totalTests, totalPassed;
  qtTest::StreamPointer err;
  QTextStream* out;
  QStack<qtTest::StreamPointer> streamStack;
  QStack<QString> traceStack;
};

QTE_IMPLEMENT_D_FUNC(qtTest)

//-----------------------------------------------------------------------------
qtTest::qtTest() : d_ptr(new qtTestPrivate)
{
  QTE_D(qtTest);

  qtTest::init();

  this->pushMessageStream(d->err);
}

//-----------------------------------------------------------------------------
qtTest::~qtTest()
{
  QTE_D(qtTest);

  (*d->err) << '\n' << d->totalPassed << " of " << d->totalTests << " passed";
  if (d->totalPassed < d->totalTests)
    {
    (*d->err) << ", " << (d->totalTests - d->totalPassed) << " failed";
    }
  (*d->err) << '\n';
  d->err->flush();

  delete d;
}

//-----------------------------------------------------------------------------
void qtTest::init()
{
  static volatile bool initialized = false;
  if (!initialized)
    {
    initialized = true;
    qInstallMsgHandler(&qtMessageHandler);
    }
}

//-----------------------------------------------------------------------------
int qtTest::result() const
{
  QTE_D_CONST(qtTest);
  return d->collectedResult;
}

//-----------------------------------------------------------------------------
int qtTest::runSuite(const QString& name, qtTest::Suite suite)
{
  QTE_D(qtTest);

  // Display banner
  ++d->totalTests;
  QString buffer(60 - name.length(), '.');
  (*d->err) << name << ' ' << buffer;

  // Register stream for QDebug handler
  buffer.clear();
  StreamPointer out(new QTextStream(&buffer));
  d->out = out.data();
  uint token = this->pushMessageStream(out);

  // Execute the test
  d->testResult = 0;
  int result = (*suite)(*this) | d->testResult;

  // Restore stream for QDebug handler
  this->popMessageStream(token);
  d->out = d->err.data();

  // Print test result and output
  (*d->err) << (result ? "***Failed" : "   Passed") << '\n' << buffer;

  // Update statistics
  if (result == 0)
    {
    ++d->totalPassed;
    }
  d->collectedResult |= result;

  return result;
}

//-----------------------------------------------------------------------------
QTextStream& qtTest::out()
{
  QTE_D(qtTest);
  return (d->out ? *d->out : *d->err);
}

//-----------------------------------------------------------------------------
uint qtTest::pushMessageStream(qtTest::StreamPointer stream)
{
  QTE_D(qtTest);

  // Register new stream with handler
  qtMessageHandler(QtSystemMsg, 0, stream);

  // Add stream to stack
  uint token = d->streamStack.count();
  d->streamStack.push(stream);
  return token;
}

//-----------------------------------------------------------------------------
void qtTest::popMessageStream(uint token)
{
  QTE_D(qtTest);

  // Remove stream(s) from stack to get back to token
  while (uint(d->streamStack.count()) > token)
    {
    d->streamStack.pop();
    }

  // Register old stream with handler
  StreamPointer stream;
  if (d->streamStack.count())
    {
    stream = d->streamStack.top();
    }
  qtMessageHandler(QtSystemMsg, 0, stream);
}

//-----------------------------------------------------------------------------
QString qtTest::where() const
{
  QTE_D_CONST(qtTest);
  if (!d->traceStack.isEmpty())
    {
    QString trace = "in " + d->traceStack.top() + '\n';
    for (int i = d->traceStack.count() - 2; i >= 0; --i)
      {
      trace += "  from " + d->traceStack[i] + '\n';
      }
    return trace;
    }
  return QString();
}

//-----------------------------------------------------------------------------
int qtTest::testEqual(float expectedValue, float actualValue,
                      const QString& exprText,
                      int line, const char* file, const char* func)
{
  if (!qFuzzyCompare(actualValue, expectedValue))
    {
    return this->failedEqualityTest(expectedValue, actualValue, exprText,
                                    line, file, func);
    }
  return 0;
}

//-----------------------------------------------------------------------------
int qtTest::testEqual(double expectedValue, double actualValue,
                      const QString& exprText,
                      int line, const char* file, const char* func)
{
  if (!qFuzzyCompare(actualValue, expectedValue))
    {
    return this->failedEqualityTest(expectedValue, actualValue, exprText,
                                    line, file, func);
    }
  return 0;
}

//-----------------------------------------------------------------------------
int qtTest::failedEqualityTest(
  const std::string& expectedValue, const std::string& actualValue,
  const QString& exprText, int line, const char* file, const char* func)
{
  return this->failedEqualityTest(qtString(expectedValue),
                                  qtString(actualValue),
                                  exprText, line, file, func);
}

//-----------------------------------------------------------------------------
int qtTest::testResult() const
{
  QTE_D_CONST(qtTest);
  return d->testResult;
}

//-----------------------------------------------------------------------------
int qtTest::setTestResult(int result)
{
  QTE_D(qtTest);
  return (d->testResult |= result);
}

//-----------------------------------------------------------------------------
class qtTestTracePrivate
{
public:
  qtTest* test;
  int depth;
};

QTE_IMPLEMENT_D_FUNC(qtTestTrace)

//-----------------------------------------------------------------------------
qtTestTrace::qtTestTrace(qtTest& test, int line, const char* file,
                         const char* func)
  : d_ptr(new qtTestTracePrivate)
{
  QTE_D(qtTestTrace);

  d->test = &test;

  if (line)
    {
    // Get trace size so we can remove ourselves later
    qtTestPrivate* td = test.d_func();
    d->depth = td->traceStack.count();

    // Get function name without return type, if function name was given
    QString shortFunc;
    if (func)
      {
      shortFunc = QString::fromLocal8Bit(func);
      if (shortFunc.contains(' '))
        {
        QRegExp re("[^ (]+([(][^)]*[)])?$");
        int n = re.indexIn(shortFunc);
        if (n > 0)
          {
          shortFunc.remove(0, n);
          }
        }
      }

    // Get short file name, if file name was given
    if (file)
      {
      QFileInfo fi(QString::fromLocal8Bit(file));
      QString shortFile = fi.fileName();
      QString trace("%2:%1");
      trace = trace.arg(line).arg(shortFile);
      if (func)
        {
        td->traceStack.push(shortFunc + " (" + trace + ')');
        }
      else
        {
        td->traceStack.push(trace);
        }
      }
    else
      {
      if (func)
        {
        QString trace("%2 (%1)");
        td->traceStack.push(trace.arg(line).arg(shortFunc));
        }
      else
        {
        td->traceStack.push(QString::number(line));
        }
      }
    }
}

//-----------------------------------------------------------------------------
qtTestTrace::~qtTestTrace()
{
  QTE_D(qtTestTrace);

  if (d->depth >= 0)
    {
    qtTestPrivate* td = d->test->d_func();
    while (td->traceStack.count() > d->depth)
      {
      td->traceStack.pop();
      }
    }

  delete d;
}
