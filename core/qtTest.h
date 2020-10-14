// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtTest_h
#define __qtTest_h

#include "qtGlobal.h"

#include <QDebug>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>

#include <cstdio>

class qtTestPrivate;
class qtTestTracePrivate;

//-----------------------------------------------------------------------------
class QTE_EXPORT qtTest
{
public:
    using StreamPointer = QSharedPointer<QTextStream>;
    using Suite = int(*)(qtTest&);

    qtTest();
    virtual ~qtTest();

    static void init();

    int result() const;
    int runSuite(QString const& name, Suite);

    QTextStream& out();
    uint pushMessageStream(StreamPointer);
    void popMessageStream(uint token);

    QString where() const;

    inline int test(
        bool exprValue, QString const& exprText, int line = 0,
        char const* file = 0, char const* func = 0);
    template <typename T>
    inline int testEqual(
        T expectedValue, T actualValue, QString const& exprText,
        int line = 0, char const* file = 0, char const* func = 0);
    int testEqual(
        float expectedValue, float actualValue, QString const& exprText,
        int line = 0, char const* file = 0, char const* func = 0);
    int testEqual(
        double expectedValue, double actualValue, QString const& exprText,
        int line = 0, char const* file = 0, char const* func = 0);

    int testResult() const;
    int setTestResult(int);

protected:
    QTE_DECLARE_PRIVATE_PTR(qtTest)
    friend class qtTestTrace;

    template <typename T>
    inline int failedEqualityTest(
        T expectedValue, T actualValue, QString const& exprText,
        int line, char const* file, char const* func);
    int failedEqualityTest(
        std::string const& expectedValue, std::string const& actualValue,
        QString const& exprText, int line, char const* file, char const* func);

private:
    QTE_DECLARE_PRIVATE(qtTest)
    QTE_DISABLE_COPY(qtTest)
};

//-----------------------------------------------------------------------------
class QTE_EXPORT qtTestTrace
{
public:
    qtTestTrace(qtTest& test, int line = 0, char const* file = 0,
                char const* func = 0);
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
    qtTestTrace _trace{TEST_OBJECT_NAME, _QT_TEST_WHERE_}; \
    _func(TEST_OBJECT_NAME, __VA_ARGS__); \
    } while(0)

#define TEST_QUIET_XFAIL(_expr) do { \
    qtTest::StreamPointer _stream(new qtStringStream); \
    auto const _token = TEST_OBJECT_NAME.pushMessageStream(_stream); \
    auto const _test = !!(_expr); \
    TEST_OBJECT_NAME.test(!_test, "(XFAIL) " #_expr, _QT_TEST_WHERE_); \
    if(_test) \
        TEST_OBJECT_NAME.out() << *_stream->string(); \
    TEST_OBJECT_NAME.popMessageStream(_token); \
    } while(0)

//-----------------------------------------------------------------------------
int qtTest::test(bool exprValue, QString const& exprText,
                 int line, char const* file, char const* func)
{
    if (!exprValue)
    {
        qtTestTrace t(*this, line, file, func);
        this->out() << this->where()
                    << "assertion (" << exprText << ") failed\n";
        return this->setTestResult(1);
    }

    return 0;
}

//-----------------------------------------------------------------------------
template <typename T>
int qtTest::testEqual(T expectedValue, T actualValue, QString const& exprText,
                      int line, char const* file, char const* func)
{
    if (actualValue != expectedValue)
    {
        return this->failedEqualityTest(expectedValue, actualValue, exprText,
                                        line, file, func);
    }
    return 0;
}

//-----------------------------------------------------------------------------
template <typename T>
int qtTest::failedEqualityTest(
    T expectedValue, T actualValue, QString const& exprText,
    int line, char const* file, char const* func)
{
    qtTestTrace t(*this, line, file, func);

    QString expectedStr;
    QDebug dbgExpected{&expectedStr};
    dbgExpected << expectedValue;
    expectedStr = expectedStr.trimmed();

    QString buffer;
    QDebug dbg{&buffer};
    dbg.nospace() << qPrintable(this->where()) << "expression " << exprText;
    dbg.space() << "gave value" << actualValue << "(expected";
    dbg.nospace() << qPrintable(expectedStr) << ")\n";
    this->out() << buffer;

    return this->setTestResult(1);
}

#endif
