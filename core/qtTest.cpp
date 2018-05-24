/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtTest.h"

#include "qtStlUtil.h"
#include "qtUtil.h"

#include <QFileInfo>
#include <QRegExp>
#include <QStack>
#include <QThreadStorage>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QMessageLogContext>
#endif

namespace // anonymous
{

//-----------------------------------------------------------------------------
static void qtMessageHandler(
    QtMsgType, QString const& msg, qtTest::StreamPointer stream)
{
    static QThreadStorage<qtTest::StreamPointer*> ts;
    qtTest::StreamPointer* tsp = ts.localData();

    if (stream || msg == 0)
    {
        if (tsp)
            (*tsp) = stream;
        else
            ts.setLocalData(new qtTest::StreamPointer(stream));
    }
    else
    {
        if (tsp && *tsp)
            (**tsp) << msg << '\n';
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

//-----------------------------------------------------------------------------
static void qtMessageHandler(
    QtMsgType type, QMessageLogContext const& context, QString const& msg)
{
    Q_UNUSED(context);
    qtMessageHandler(type, msg, qtTest::StreamPointer());
}

#else

//-----------------------------------------------------------------------------
static void qtMessageHandler(QtMsgType type, char const* msg)
{
    qtMessageHandler(type, QString::fromLocal8Bit(msg),
                     qtTest::StreamPointer());
}

#endif

} // namespace <anonymous>

//-----------------------------------------------------------------------------
class qtTestPrivate
{
public:
    qtTestPrivate() : err{new QTextStream(stderr, QIODevice::WriteOnly)}
    {}

    int testResult = 0, collectedResult = 0;
    int totalTests = 0, totalPassed = 0;
    qtTest::StreamPointer err;
    QTextStream* out;
    QStack<qtTest::StreamPointer> streamStack;
    QStack<QString> traceStack;
};

QTE_IMPLEMENT_D_FUNC(qtTest)

//-----------------------------------------------------------------------------
qtTest::qtTest() : d_ptr{new qtTestPrivate}
{
    QTE_D();

    qtTest::init();

    this->pushMessageStream(d->err);
}

//-----------------------------------------------------------------------------
qtTest::~qtTest()
{
    QTE_D();

    (*d->err) << '\n' << d->totalPassed << " of " << d->totalTests << " passed";
    if (d->totalPassed < d->totalTests)
        (*d->err) << ", " << (d->totalTests - d->totalPassed) << " failed";
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        qInstallMessageHandler(&qtMessageHandler);
#else
        qInstallMsgHandler(&qtMessageHandler);
#endif
    }
}

//-----------------------------------------------------------------------------
int qtTest::result() const
{
    QTE_D();
    return d->collectedResult;
}

//-----------------------------------------------------------------------------
int qtTest::runSuite(QString const& name, qtTest::Suite suite)
{
    QTE_D();

    // Display banner
    ++d->totalTests;
    QString buffer{60 - name.length(), '.'};
    (*d->err) << name << ' ' << buffer;

    // Register stream for QDebug handler
    buffer.clear();
    StreamPointer out{new QTextStream{&buffer}};
    d->out = out.data();
    auto const token = this->pushMessageStream(out);

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
        ++d->totalPassed;
    d->collectedResult |= result;

    return result;
}

//-----------------------------------------------------------------------------
QTextStream& qtTest::out()
{
    QTE_D();
    return (d->out ? *d->out : *d->err);
}

//-----------------------------------------------------------------------------
uint qtTest::pushMessageStream(qtTest::StreamPointer stream)
{
    QTE_D();

    // Register new stream with handler
    qtMessageHandler(QtSystemMsg, 0, stream);

    // Add stream to stack
    uint token = static_cast<uint>(d->streamStack.count());
    d->streamStack.push(stream);
    return token;
}

//-----------------------------------------------------------------------------
void qtTest::popMessageStream(uint token)
{
    QTE_D();

    // Remove stream(s) from stack to get back to token
    while (static_cast<uint>(d->streamStack.count()) > token)
        d->streamStack.pop();

    // Register old stream with handler
    StreamPointer stream;
    if (d->streamStack.count())
        stream = d->streamStack.top();
    qtMessageHandler(QtSystemMsg, 0, stream);
}

//-----------------------------------------------------------------------------
QString qtTest::where() const
{
    QTE_D();
    if (!d->traceStack.isEmpty())
    {
        QString trace = "in " + d->traceStack.top() + '\n';
        for (int i = d->traceStack.count() - 2; i >= 0; --i)
            trace += "  from " + d->traceStack[i] + '\n';
        return trace;
    }
  return {};
}

//-----------------------------------------------------------------------------
int qtTest::testEqual(float expectedValue, float actualValue,
                      QString const& exprText,
                      int line, char const* file, char const* func)
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
                      QString const& exprText,
                      int line, char const* file, char const* func)
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
    std::string const& expectedValue, std::string const& actualValue,
    QString const& exprText, int line, char const* file, char const* func)
{
    return this->failedEqualityTest(qtString(expectedValue),
                                    qtString(actualValue),
                                    exprText, line, file, func);
}

//-----------------------------------------------------------------------------
int qtTest::testResult() const
{
    QTE_D();
    return d->testResult;
}

//-----------------------------------------------------------------------------
int qtTest::setTestResult(int result)
{
    QTE_D();
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
qtTestTrace::qtTestTrace(qtTest& test, int line, char const* file,
                         char const* func)
    : d_ptr(new qtTestTracePrivate)
{
    QTE_D();

    d->test = &test;

    if (line)
    {
        // Get trace size so we can remove ourselves later
        auto* const td = test.d_func();
        d->depth = td->traceStack.count();

        // Get function name without return type, if function name was given
        QString shortFunc;
        if (func)
        {
            shortFunc = QString::fromLocal8Bit(func);
            if (shortFunc.contains(' '))
            {
                QRegExp const re{"[^ (]+([(][^)]*[)])?$"};
                auto const n = re.indexIn(shortFunc);
                if (n > 0)
                    shortFunc.remove(0, n);
            }
        }

        // Get short file name, if file name was given
        if (file)
        {
            QFileInfo const fi{QString::fromLocal8Bit(file)};
            auto const& shortFile = fi.fileName();
            auto trace = QString{"%2:%1"};
            trace = trace.arg(line).arg(shortFile);
            if (func)
                td->traceStack.push(shortFunc + " (" + trace + ')');
            else
                td->traceStack.push(trace);
        }
        else
        {
            if (func)
            {
                auto const trace = QString{"%2 (%1)"};
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
    QTE_D();

    if (d->depth >= 0)
    {
        qtTestPrivate* td = d->test->d_func();
        while (td->traceStack.count() > d->depth)
            td->traceStack.pop();
    }

    delete d;
}
