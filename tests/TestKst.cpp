/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <cstdio>

#define TEST_OBJECT_NAME t_obj

#include "../core/qtTest.h"
#include "../core/qtMath.h"

#include "../io/qtKstParser.h"
#include "../io/qtKstReader.h"
#include "../io/qtStringStream.h"

QUrl testFile;

//-----------------------------------------------------------------------------
bool compareReal(double a, double b)
{
  // Fail if either value is NaN
  if (qIsNaN(a) || qIsNaN(b))
    return false;

  // Return true if both values are infinite and of the same sign; technically
  // such a comparison is meaningless, but the purpose of this test is to
  // validate that parseReal returns infinity when we expect it to
  if (qIsInf(a) && qIsInf(b))
    return (a > 0.0) == (b > 0.0);

  // For finite numbers, use Qt's fuzzy comparison (which basically tests for
  // equality within a very small epsilon relative to the magnitude of the
  // numbers being tested)
  return qFuzzyCompare(a, b);
}

//-----------------------------------------------------------------------------
int testLong(qtTest& t_obj, const QString& str, qint64 xpv, bool xrv = true)
{
  qint64 apv;
  bool arv = qtKstParser::parseLong(str.toLower(), apv);

  // Check return value
  if (arv != xrv)
    {
    t_obj.out() << t_obj.where() << "parseLong(\"" << str << "\") ";
    if (xrv)
      t_obj.out() << "failed (expected success)\n";
    else
      t_obj.out() << "succeeded (expected failure)\n";
    return t_obj.setTestResult(1);
    }

  // Check parsed value
  if (xrv && apv != xpv)
    {
    t_obj.out() << t_obj.where() << "parseLong(\"" << str << "\")"
                << " gave value " << apv << " (expected " << xpv << ")\n";
    return t_obj.setTestResult(1);
    }

  return 0;
}

//-----------------------------------------------------------------------------
int testReal(qtTest& t_obj, const QString& str, double xpv, bool xrv = true)
{
  double apv;
  bool arv = qtKstParser::parseReal(str.toLower(), apv);

  // Check return value
  if (arv != xrv)
    {
    t_obj.out() << t_obj.where() << "parseReal(\"" << str << "\") ";
    if (xrv)
      t_obj.out() << "failed (expected success)\n";
    else
      t_obj.out() << "succeeded (expected failure)\n";
    return t_obj.setTestResult(1);
    }

  // Check parsed value
  if (xpv && !compareReal(apv, xpv))
    {
    t_obj.out() << t_obj.where() << "parseReal(\"" << str << "\")"
                << " gave value " << apv << "(expected " << xpv << ")\n";
    return t_obj.setTestResult(1);
    }

  return 0;
}

//-----------------------------------------------------------------------------
int testParse(qtTest& t_obj, const QString& data, const QString& xsv,
              bool xrv = true)
{
  qtKstReader r(data);
  if (xrv != r.isValid())
    {
    t_obj.out() << t_obj.where() << "testParse(\"" << data << "\") ";
    if (xrv)
      t_obj.out() << "produced an invalid reader (expected a valid reader)\n";
    else
      t_obj.out() << "produced a valid reader (expected an invalid reader)\n";
    return t_obj.setTestResult(1);
    }

  if (r.isValid())
    {
    QString asv;
    if (!r.readString(asv))
      {
      t_obj.out() << t_obj.where() << "testParse(\"" << data << "\")"
                  << " failed to read value\n";
      return t_obj.setTestResult(1);
      }
    if (asv != xsv)
      {
      t_obj.out() << t_obj.where() << "testParse(\"" << data << "\")"
                  << " got value \"" << asv << "\" "
                  << "(expected " << xsv << ")\n";
      return t_obj.setTestResult(1);
      }
    }

  return 0;
}

//-----------------------------------------------------------------------------
int testInvalid(qtTest& t_obj, const QString& data)
{
  // Set up stream to catch any qDebug() output from the KST parsing
  qtTest::StreamPointer stream(new qtStringStream);
  uint token = t_obj.pushMessageStream(stream);

  // Execute the test
  qtTestTrace t(TEST_OBJECT_NAME, __LINE__ + 1, __FILE__, _QT_TEST_FUNCTION_);
  int result = testParse(t_obj, data, QString(), false);

  // If the KST was unexpectedly parsed successfully, emit any output
  if (result)
    t_obj.out() << *stream->string();

  // Restore the stream and return the result
  t_obj.popMessageStream(token);
  return result;
}

//-----------------------------------------------------------------------------
int testBasicLong(qtTest& t_obj)
{
  TEST_CALL(testLong, "12345", 12345);
  TEST_CALL(testLong, "+12345", 12345);
  TEST_CALL(testLong, "-12345", -12345);
  TEST_CALL(testLong, "1.2345e4", 12345);
  TEST_CALL(testLong, "1.2345^4", 12345);
  TEST_CALL(testLong, "123450e-1", 12345);
  TEST_CALL(testLong, "0x1.2345^4", 0x12345);
  TEST_CALL(testLong, "0x1,2345p4", 0x12345);
  TEST_CALL(testLong, "-5.36705214244372261e+21b8", -12345678987654321LL);
  TEST_CALL(testLong, "z.xq551^7@36", 2816160313680LL);
  TEST_CALL(testLong, "-z,xq551^7@36", -2816160313680LL);
  TEST_CALL(testLong, "zzz^-1@36", 1295);
  TEST_CALL(testLong, "1e010", 10000000000);
  return 0;
}

//-----------------------------------------------------------------------------
int testBasicReal(qtTest& t_obj)
{
  TEST_CALL(testReal, "12345.67", 12345.67);
  TEST_CALL(testReal, "12345,67", 12345.67);
  TEST_CALL(testReal, "+12345.67", 12345.67);
  TEST_CALL(testReal, "-12345.67", -12345.67);
  TEST_CALL(testReal, "1.234567e4", 12345.67);
  TEST_CALL(testReal, "1.234567^4", 12345.67);
  TEST_CALL(testReal, "12345670e-3", 12345.67);
  TEST_CALL(testReal, "0x1.234567^4", 74565.40234375);
  TEST_CALL(testReal, "0x1,234567p4", 74565.40234375);
  TEST_CALL(testReal, "-1.3376673360061e+13b8", -12345653120.765625);
  TEST_CALL(testReal, "z.xq551^3@36", 1676669.139660494);
  TEST_CALL(testReal, "-z,xq551^3@36", -1676669.139660494);
  TEST_CALL(testReal, "zzz^-1@36", 1295.972222222222);
  TEST_CALL(testReal, "1e010", 1e10);
  return 0;
}

//-----------------------------------------------------------------------------
int testArcLong(qtTest& t_obj)
{
  TEST_CALL(testLong,  "55d12'9.21\"",   55);
  TEST_CALL(testLong, "+55d12'9.21\"",   55);
  TEST_CALL(testLong, "-55d12'9.21\"",  -55);
  TEST_CALL(testLong,  "55d12'9.21\"N",  55);
  TEST_CALL(testLong,  "55d12'9.21\"S", -55);
  TEST_CALL(testLong,  "55d12'9.21\"E",  55);
  TEST_CALL(testLong,  "55d12'9.21\"W", -55);
  TEST_CALL(testLong, "655'", 10);
  TEST_CALL(testLong, "7288\"", 2);
  return 0;
}

//-----------------------------------------------------------------------------
int testArcReal(qtTest& t_obj)
{
  TEST_CALL(testReal,  "55d12'9.21\"",   55.202558333333333);
  TEST_CALL(testReal, "+55d12'9.21\"",   55.202558333333333);
  TEST_CALL(testReal, "-55d12'9.21\"",  -55.202558333333333);
  TEST_CALL(testReal,  "55d12'9.21\"N",  55.202558333333333);
  TEST_CALL(testReal,  "55d12'9.21\"S", -55.202558333333333);
  TEST_CALL(testReal,  "55d12'9.21\"E",  55.202558333333333);
  TEST_CALL(testReal,  "55d12'9.21\"W", -55.202558333333333);
  TEST_CALL(testReal, "655'", 10.91666666666667);
  TEST_CALL(testReal, "7288\"", 2.024444444444444);
  return 0;
}

//-----------------------------------------------------------------------------
int testBasicFailure(qtTest& t_obj)
{
  // Long
  TEST_CALL(testLong, "1234c.678", 0, false);
  TEST_CALL(testLong, "12345.67a", 0, false);
  TEST_CALL(testLong, "12345. 78", 0, false);
  TEST_CALL(testLong, " 2345.678", 0, false);
  TEST_CALL(testLong, "12 45.678", 0, false);
  TEST_CALL(testLong, "128b8", 0, false);
  TEST_CALL(testLong, "127e18b8", 0, false);
  TEST_CALL(testLong, "128b1", 0, false);
  TEST_CALL(testLong, "128b11", 0, false);
  TEST_CALL(testLong, "128@1", 0, false);
  TEST_CALL(testLong, "128@37", 0, false);
  TEST_CALL(testLong, "12345e0x10", 0, false);
  // Real
  TEST_CALL(testReal, "1234c.678", 0, false);
  TEST_CALL(testReal, "12345.67a", 0, false);
  TEST_CALL(testReal, "12345. 78", 0, false);
  TEST_CALL(testReal, " 2345.678", 0, false);
  TEST_CALL(testReal, "12 45.678", 0, false);
  TEST_CALL(testReal, "128b8", 0, false);
  TEST_CALL(testReal, "127e18b8", 0, false);
  TEST_CALL(testReal, "128b1", 0, false);
  TEST_CALL(testReal, "128b11", 0, false);
  TEST_CALL(testReal, "128@1", 0, false);
  TEST_CALL(testReal, "128@37", 0, false);
  TEST_CALL(testReal, "12345e0x10", 0, false);
  return 0;
}

//-----------------------------------------------------------------------------
int testArcFailure(qtTest& t_obj)
{
  // Long
  TEST_CALL(testLong,  "5cd12'9.21\"",  0, false);
  TEST_CALL(testLong,  "55d1a'9.21\"",  0, false);
  TEST_CALL(testLong,  "55d12'9.q1\"",  0, false);
  TEST_CALL(testLong, "+55d12'9.21\"N", 0, false);
  TEST_CALL(testLong, "-55d12'9.21\"N", 0, false);
  // Real
  TEST_CALL(testReal,  "5cd12'9.21\"",  0, false);
  TEST_CALL(testReal,  "55d1a'9.21\"",  0, false);
  TEST_CALL(testReal,  "55d12'9.q1\"",  0, false);
  TEST_CALL(testReal, "+55d12'9.21\"N", 0, false);
  TEST_CALL(testReal, "-55d12'9.21\"N", 0, false);
  return 0;
}

//-----------------------------------------------------------------------------
int testOverflowUnderflow(qtTest& t_obj)
{
  // Long
  TEST_CALL(testLong,  "0x7f^e", 0x7F00000000000000LL);
  TEST_CALL(testLong, "-0x80^e", 0x8000000000000000LL);
  TEST_CALL(testLong,  "0x80^e", 0, false);
  TEST_CALL(testLong, "-0x81^e", 0, false);
  TEST_CALL(testLong, "1e19", 0, false);
  TEST_CALL(testLong, "1e-1", 0);
  TEST_CALL(testLong,  "9223372036854775807", 0x7FFFFFFFFFFFFFFFLL);
  TEST_CALL(testLong,  "9223372036854775808", 0, false);
  TEST_CALL(testLong, "-9223372036854775808", 0x8000000000000000LL);
  TEST_CALL(testLong, "-9223372036854775809", 0, false);
  // Real
  TEST_CALL(testReal,  "1e400",  qInf());
  TEST_CALL(testReal, "-1e400", -qInf());
  TEST_CALL(testReal,  "1e2000b8",  qInf());
  TEST_CALL(testReal, "-1e2000b8", -qInf());
  TEST_CALL(testReal,  "1e-400", 0.0);
  TEST_CALL(testReal, "-1e-400", 0.0);
  QString bigNum = QString().fill('9', 400);
  TEST_CALL(testReal,       bigNum,  qInf());
  TEST_CALL(testReal, "-" + bigNum, -qInf());
  return 0;
}

//-----------------------------------------------------------------------------
int testValidity(qtTest& t_obj)
{
  TEST_CALL(testInvalid, "");
  TEST_CALL(testInvalid, ",");
  TEST_CALL(testInvalid, "1");
  TEST_CALL(testInvalid, "[;");
  TEST_CALL(testInvalid, "\"");
  TEST_CALL(testInvalid, "\"\\");
  TEST_CALL(testInvalid, "\"\"");
  TEST_CALL(testInvalid, "[1] j;");
  TEST_CALL(testInvalid, "[1] \"\";");
  TEST_CALL(testInvalid, "[1][]");
  TEST_CALL(testInvalid, "#\n");
  TEST_CALL(testInvalid, ";,\n#\n");
  TEST_CALL(testInvalid, "1#;\n");

  return 0;
}

//-----------------------------------------------------------------------------
int testBasicParsing(qtTest& t_obj)
{
  TEST_CALL(testParse, ";", "");
  TEST_CALL(testParse, ",;", "");
  TEST_CALL(testParse, "1;", "1");
  TEST_CALL(testParse, "[];", "");
  TEST_CALL(testParse, "[1];", "1");
  TEST_CALL(testParse, "[[[[1]]]];", "1");
  TEST_CALL(testParse, "\"\";", "");
  TEST_CALL(testParse, "\"\\\"\";", "\"");
  TEST_CALL(testParse, "\"1\\\n2\";", "12");
  TEST_CALL(testParse, "1 2;", "12");
  TEST_CALL(testParse, "1 [2];", "1[2]");
  TEST_CALL(testParse, "\\ \"\\\\\\q\\x05\";", "\\\\qx05");
  TEST_CALL(testParse, "3\" \" pipe wrench\";", "3\" pipe wrench");
  TEST_CALL(testParse, "\t\n  ;  \t\n", "");
  TEST_CALL(testParse, "hello; # world?\n", "hello");
  TEST_CALL(testParse, "# a\n 1 # b\n # c\n 2; # d\n", "12");
  TEST_CALL(testParse, "1#;[]\"\n;", "1");
  TEST_CALL(testParse, "\"a # b\"; # c\n", "a # b");

  return 0;
}

//-----------------------------------------------------------------------------
int testCountSeek(qtTest& t_obj)
{
  qtKstReader r("1, 2; 3; 4, 5, 6, 7; 8; ; ,,; ");
  if (TEST(r.isValid()))
    return 1;

  TEST_EQUAL(r.recordCount(), 6);
  TEST_EQUAL(r.valueCount(0), 2);
  TEST_EQUAL(r.valueCount(1), 1);
  TEST_EQUAL(r.valueCount(2), 4);
  TEST_EQUAL(r.valueCount(3), 1);
  TEST_EQUAL(r.valueCount(4), 1);
  TEST_EQUAL(r.valueCount(5), 3);

  TEST(r.isEndOfRecord(1, 0) == false);
  TEST(r.isEndOfRecord(1, 1) == true);
  TEST(r.isEndOfFile(5) == false);
  TEST(r.isEndOfFile(6) == true);

  TEST_EQUAL(r.currentRecord(), 0);
  TEST_EQUAL(r.currentValue(), 0);

  TEST(r.nextValue());
  TEST_EQUAL(r.currentRecord(), 0);
  TEST_EQUAL(r.currentValue(), 1);
  TEST(!r.isEndOfRecord());
  TEST(r.nextValue());
  TEST_EQUAL(r.currentRecord(), 0);
  TEST_EQUAL(r.currentValue(), 2);
  TEST(r.isEndOfRecord());
  TEST(!r.nextValue());

  TEST(r.nextRecord());
  TEST_EQUAL(r.currentRecord(), 1);
  TEST_EQUAL(r.currentValue(), 0);
  TEST(r.seek(5, 1));
  TEST_EQUAL(r.currentRecord(), 5);
  TEST_EQUAL(r.currentValue(), 1);
  TEST(!r.isEndOfRecord());
  TEST(!r.isEndOfFile());
  TEST(!r.seek(5, 5));
  TEST_EQUAL(r.currentRecord(), 5);
  TEST_EQUAL(r.currentValue(), 1);
  TEST(!r.seek(6, 0));
  TEST_EQUAL(r.currentRecord(), 5);
  TEST_EQUAL(r.currentValue(), 1);
  TEST(r.nextRecord());
  TEST_EQUAL(r.currentRecord(), 6);
  TEST_EQUAL(r.currentValue(), 0);
  TEST(r.isEndOfFile());
  TEST(!r.nextRecord());

  return 0;
}

//-----------------------------------------------------------------------------
int testArray(qtTest& t_obj)
{
  qtKstReader r("[ 1, [ 2, 3, 4 ], [ 5, [ 6, 7 ], [ [ 8 ], 9 ] ] ];"
                "[ [ 11 ], [ 12 ], [ 13, 14 ] ];");
  if (TEST(r.isValid()))
    return 1;

  TEST_EQUAL(r.recordCount(), 2);
  TEST_EQUAL(r.valueCount(0), 1);
  TEST_EQUAL(r.valueCount(1), 1);

  int iv;
  QList<int> ia;
  qtKstReader ra1;
  qtKstReader ra2;
  qtKstReader rt;

  // Reading value as array
  if (TEST(!ra1.isValid()))
    return 1;
  TEST(r.readArray(ra1));
  TEST_EQUAL(ra1.recordCount(), 1);
  TEST_EQUAL(ra1.valueCount(0), 3);

  // Can't read non-array as array
  TEST(!ra1.readArray(ra2));

  // Reading values from array
  TEST(ra1.readInt(iv));
  TEST_EQUAL(iv, 1);
  TEST(ra1.nextValue());
  TEST(ra1.readIntArray(ia));
  TEST_EQUAL(ia.count(), 3);
  TEST_EQUAL(ia[0], 2);
  TEST_EQUAL(ia[1], 3);
  TEST_EQUAL(ia[2], 4);
  TEST(ra1.nextValue());

  // Can't read array with nested array
  TEST(!ra1.readIntArray(ia));

  // Reading multiply-nested array
  TEST(ra1.readArray(ra2));
  TEST_EQUAL(ra2.recordCount(), 1);
  TEST_EQUAL(ra2.valueCount(0), 3);

  // Can't read into already-initialized reader
  TEST(!ra1.readArray(ra2));

  // Reading values from multiply-nested array
  TEST(ra2.readInt(iv));
  TEST_EQUAL(iv, 5);
  TEST(ra2.nextValue());
  TEST(ra2.readIntArray(ia));
  TEST_EQUAL(ia.count(), 2);
  TEST_EQUAL(ia[0], 6);
  TEST_EQUAL(ia[1], 7);
  TEST(ra2.nextValue());

  // Reading as array when nested array is one-value
  TEST(ra2.readIntArray(ia));
  TEST_EQUAL(ia.count(), 2);
  TEST_EQUAL(ia[0], 8);
  TEST_EQUAL(ia[1], 9);
  TEST(ra2.nextValue());

  // End of record on nested array
  TEST(ra2.isEndOfRecord());
  TEST(ra2.nextRecord());
  TEST(ra2.isEndOfFile());

  // Can't read not-table as table
  TEST(!r.readTable(rt));

  // Reading value as table
  TEST(r.nextRecord());
  TEST(r.readTable(rt));
  TEST_EQUAL(rt.recordCount(), 3);
  TEST_EQUAL(rt.valueCount(0), 1);
  TEST_EQUAL(rt.valueCount(1), 1);
  TEST_EQUAL(rt.valueCount(2), 2);

  // Reading values from table
  TEST(rt.readInt(iv));
  TEST_EQUAL(iv, 11);
  TEST(rt.nextValue());
  TEST(rt.isEndOfRecord());
  TEST(rt.nextRecord());
  TEST(rt.readInt(iv));
  TEST_EQUAL(iv, 12);
  TEST(rt.nextRecord());
  TEST(rt.readInt(iv));
  TEST_EQUAL(iv, 13);
  TEST(rt.nextValue());
  TEST(rt.readInt(iv));
  TEST_EQUAL(iv, 14);
  TEST(rt.nextRecord());
  TEST(rt.isEndOfFile());

  return 0;
}

//-----------------------------------------------------------------------------
int testEmpty(qtTest& t_obj)
{
  qtKstReader r(", 1, [], [1];");
  if (TEST(r.isValid()))
    return 1;

  TEST_EQUAL(r.isValueEmpty(0), true);
  TEST_EQUAL(r.isValueEmpty(1), false);
  TEST_EQUAL(r.isValueEmpty(2), false);
  TEST_EQUAL(r.isValueEmpty(3), false);

  TEST_EQUAL(r.isArrayEmpty(0), true);
  TEST_EQUAL(r.isArrayEmpty(1), false);
  TEST_EQUAL(r.isArrayEmpty(2), true);
  TEST_EQUAL(r.isArrayEmpty(3), false);

  return 0;
}

//-----------------------------------------------------------------------------
int testFileValues(qtTest& t_obj)
{
  qtKstReader r(testFile);
  if (TEST(r.isValid()))
    return 1;

  TEST_EQUAL(r.recordCount(), 5);
  TEST_EQUAL(r.valueCount(0), 3);
  TEST_EQUAL(r.valueCount(1), 2);
  TEST_EQUAL(r.valueCount(2), 1);
  TEST_EQUAL(r.valueCount(3), 1);
  TEST_EQUAL(r.valueCount(4), 1);

  int iv;
  double rv;
  QList<int> ia;
  QList<qint64> la;
  QList<double> ra;

  // Reading array with one value as 1-value and as array
  TEST(r.readInt(iv));
  TEST_EQUAL(iv, 1);
  TEST(r.readIntArray(ia));
  TEST_EQUAL(ia.count(), 1);
  TEST_EQUAL(ia[0], 1);
  TEST(r.nextValue());

  // Reading int array
  TEST(r.readIntArray(ia));
  TEST_EQUAL(ia.count(), 3);
  TEST_EQUAL(ia[0], 2);
  TEST_EQUAL(ia[1], 3);
  TEST_EQUAL(ia[2], 4);
  TEST(r.nextValue());

  // Can't read nested array
  TEST(!r.readIntArray(ia));
  TEST(r.nextRecord());

  // Overflow test of int
  TEST(r.readInt(iv));
  TEST_EQUAL(iv, 2147483647);
  TEST(r.nextValue());
  TEST(!r.readInt(iv));
  TEST(r.nextRecord());

  // Reading UTF-8 arc length
  TEST(r.readReal(rv));
  TEST_EQUAL(rv, 55.202558333333333);
  TEST(r.nextRecord());

  // Reading long array
  TEST(r.readLongArray(la));
  TEST_EQUAL(la.count(), 2);
  TEST_EQUAL(la[0], 12345678987654321LL);
  TEST_EQUAL(la[1], 14725836963852741LL);
  TEST(r.nextRecord());

  // Reading real array
  TEST(r.readRealArray(ra));
  TEST_EQUAL(ra.count(), 2);
  TEST_EQUAL(ra[0], 3.1416);
  TEST_EQUAL(ra[1], 55.202558333333333);
  TEST(r.readIntArray(ia));
  TEST_EQUAL(ia.count(), 2);
  TEST_EQUAL(ia[0], 3);
  TEST_EQUAL(ia[1], 55);
  TEST(r.nextRecord());

  return 0;
}

//-----------------------------------------------------------------------------
int testSeparator(qtTest& t_obj)
{
  qtKstReader r("1 2  3\n4\t5\n", QRegExp("\\s+"), QRegExp("\n"));
  if (TEST(r.isValid()))
    return 1;

  TEST_EQUAL(r.recordCount(), 2);
  TEST_EQUAL(r.valueCount(0), 3);
  TEST_EQUAL(r.valueCount(1), 2);

  return 0;
}

//-----------------------------------------------------------------------------
int main(int argc, const char* argv[])
{
  qtTest t_obj;

  if (argc < 2)
    {
    t_obj.out() << "invocation error, path to test data file required\n";
    return 1;
    }
  testFile = QUrl::fromLocalFile(argv[1]);

  // Parser tests
  t_obj.runSuite("Basic Long Tests", testBasicLong);
  t_obj.runSuite("Basic Real Tests", testBasicReal);
  t_obj.runSuite("Arc Long Tests", testArcLong);
  t_obj.runSuite("Arc Real Tests", testArcReal);
  t_obj.runSuite("Basic Failure Tests", testBasicFailure);
  t_obj.runSuite("Arc Failure Tests", testArcFailure);
  t_obj.runSuite("Overflow/Underflow Tests", testOverflowUnderflow);
  // Reader tests
  t_obj.runSuite("Reader Validity Tests", testValidity);
  t_obj.runSuite("Reader Basic Parsing Tests", testBasicParsing);
  t_obj.runSuite("Reader Count/Seek Tests", testCountSeek);
  t_obj.runSuite("Reader Array Tests", testArray);
  t_obj.runSuite("Reader Empty Tests", testEmpty);
  t_obj.runSuite("Reader File Value Tests", testFileValues);
  t_obj.runSuite("Reader Separator Tests", testSeparator);
  return t_obj.result();
}
