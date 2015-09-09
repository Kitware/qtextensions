/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtTest_h
#define __qtTest_h

#include <QDebug>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>

#include <cstdio>

#include "qtGlobal.h"

class qtTestPrivate;
class qtTestTracePrivate;

//-----------------------------------------------------------------------------
class QTE_EXPORT qtTest
{
public:
  typedef QSharedPointer<QTextStream> StreamPointer;
  typedef int (* Suite)(qtTest&);

  qtTest();
  virtual ~qtTest();

  static void init();

  int result() const;
  int runSuite(const QString& name, Suite);

  QTextStream& out();
  uint pushMessageStream(StreamPointer);
  void popMessageStream(uint token);

  QString where() const;

  inline int test(
    bool exprValue, const QString& exprText, int line = 0,
    const char* file = 0, const char* func = 0);
  template <typename T>
  inline int testEqual(
    T expectedValue, T actualValue, const QString& exprText,
    int line = 0, const char* file = 0, const char* func = 0);
  int testEqual(
    float expectedValue, float actualValue, const QString& exprText,
    int line = 0, const char* file = 0, const char* func = 0);
  int testEqual(
    double expectedValue, double actualValue, const QString& exprText,
    int line = 0, const char* file = 0, const char* func = 0);

  int testResult() const;
  int setTestResult(int);

protected:
  QTE_DECLARE_PRIVATE_PTR(qtTest)
  friend class qtTestTrace;

  template <typename T>
  inline int failedEqualityTest(
    T expectedValue, T actualValue, const QString& exprText,
    int line, const char* file, const char* func);
  int failedEqualityTest(
    const std::string& expectedValue, const std::string& actualValue,
    const QString& exprText, int line, const char* file, const char* func);

private:
  QTE_DECLARE_PRIVATE(qtTest)
  Q_DISABLE_COPY(qtTest)
};

//-----------------------------------------------------------------------------
class QTE_EXPORT qtTestTrace
{
public:
  qtTestTrace(qtTest& test, int line = 0, const char* file = 0,
              const char* func = 0);
  ~qtTestTrace();

protected:
  QTE_DECLARE_PRIVATE_PTR(qtTestTrace)

private:
  QTE_DECLARE_PRIVATE(qtTestTrace)
};

//-----------------------------------------------------------------------------

#ifndef _QT_TEST_FUNCTION_
#if defined Q_CC_MSVC
#define _QT_TEST_FUNCTION_ __FUNCDNAME__
#elif defined Q_CC_GNU
#define _QT_TEST_FUNCTION_ __PRETTY_FUNCTION__
#else
#define _QT_TEST_FUNCTION_ __FUNCTION__
#endif
#endif

#ifndef TEST_OBJECT_NAME
#define TEST_OBJECT_NAME testObject
#endif

#define _QT_TEST_WHERE_ \
  __LINE__, __FILE__, _QT_TEST_FUNCTION_

#define TEST(_expr) \
  TEST_OBJECT_NAME.test((_expr), #_expr, _QT_TEST_WHERE_)
#define TEST_EQUAL(_expr, _value) \
  TEST_OBJECT_NAME.testEqual((_value), (_expr), #_expr, _QT_TEST_WHERE_)
#define TEST_CALL(_func, ...) do { \
  qtTestTrace _trace(TEST_OBJECT_NAME, _QT_TEST_WHERE_); \
  _func(TEST_OBJECT_NAME, __VA_ARGS__); \
  } while(0)

#define TEST_QUIET_XFAIL(_expr) do { \
  qtTest::StreamPointer _stream(new qtStringStream); \
  uint _token = TEST_OBJECT_NAME.pushMessageStream(_stream); \
  bool _test = (_expr); \
  TEST_OBJECT_NAME.test(!_test, "(XFAIL) " #_expr, _QT_TEST_WHERE_); \
  if(_test) \
    TEST_OBJECT_NAME.out() << *_stream->string(); \
  TEST_OBJECT_NAME.popMessageStream(_token); \
  } while(0)

//-----------------------------------------------------------------------------
int qtTest::test(bool exprValue, const QString& exprText,
                 int line, const char* file, const char* func)
{
  if (!exprValue)
    {
    qtTestTrace t(*this, line, file, func);
    this->out() << this->where() << "assertion (" << exprText << ") failed\n";
    return this->setTestResult(1);
    }

  return 0;
}

//-----------------------------------------------------------------------------
template <typename T>
int qtTest::testEqual(T expectedValue, T actualValue, const QString& exprText,
                      int line, const char* file, const char* func)
{
  if (actualValue != expectedValue)
    return this->failedEqualityTest(expectedValue, actualValue, exprText,
                                    line, file, func);
  return 0;
}

//-----------------------------------------------------------------------------
template <typename T>
int qtTest::failedEqualityTest(
  T expectedValue, T actualValue, const QString& exprText,
  int line, const char* file, const char* func)
{
  qtTestTrace t(*this, line, file, func);

  QString expectedStr;
  QDebug dbgExpected(&expectedStr);
  dbgExpected << expectedValue;
  expectedStr = expectedStr.trimmed();

  QString buffer;
  QDebug dbg(&buffer);
  dbg.nospace() << qPrintable(this->where()) << "expression " << exprText;
  dbg.space() << "gave value" << actualValue << "(expected";
  dbg.nospace() << qPrintable(expectedStr) << ")\n";
  this->out() << buffer;

  return this->setTestResult(1);
}

#endif
