// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#define TEST_OBJECT_NAME t_obj

#include "../core/qtTest.h"
#include "../util/qtNaturalSort.h"

#include <QDebug>
#include <QTextCodec>

using namespace qtNaturalSort;

// U+1D7FB mathematical monospace digit '5'
static auto const& special5 = QString::fromUtf8(u8"\xf0\x9d\x9f\xbb");

//-----------------------------------------------------------------------------
class ExpectedStringGroup
{
public:
    ExpectedStringGroup() = default;
    ExpectedStringGroup(ExpectedStringGroup const&) = default;

    ExpectedStringGroup(QString const& c, int l = 0, int s = 0)
        : characters{c}, leadingZeros{l}, significantDigits{s}
    {}

    ExpectedStringGroup(StringGroup const& g)
        : characters{g.characters.toString()},
          leadingZeros{g.leadingZeros},
          significantDigits{g.significantDigits}
    {}

    bool operator==(ExpectedStringGroup const& other)
    {
        return characters == other.characters &&
               leadingZeros == other.leadingZeros &&
               significantDigits == other.significantDigits;
    }

protected:
    friend QDebug& operator<<(QDebug&, ExpectedStringGroup const&);

    QString characters;
    int leadingZeros = 0;
    int significantDigits = 0;
};

//-----------------------------------------------------------------------------
QDebug& operator<<(QDebug& dbg, ExpectedStringGroup const& g)
{
  dbg << '{' << g.characters;
  if (g.significantDigits || g.leadingZeros)
    dbg << ", " << g.leadingZeros << ", " << g.significantDigits;
  dbg << '}';
  return dbg;
}

//-----------------------------------------------------------------------------
QList<ExpectedStringGroup> adapt(QVector<StringGroup> const& v)
{
    QList<ExpectedStringGroup> out;
    foreach (auto const& g, v)
        out.append(g);

    return out;
}

//-----------------------------------------------------------------------------
void testCrack(qtTest& t_obj, QString const& s,
               QList<ExpectedStringGroup> const& x)
{
    TEST_EQUAL(adapt(crack(s)), x);
}

//-----------------------------------------------------------------------------
void testCrackVar(qtTest& t_obj, QString const& s, QString const& v,
                  QList<ExpectedStringGroup> const& x,
                  QList<ExpectedStringGroup> const& xPre,
                  QList<ExpectedStringGroup> const& xPost)
{
    TEST_CALL(testCrack, v.arg(s), xPre + x + xPost);
}

//-----------------------------------------------------------------------------
void testCrackVars(qtTest& t_obj, QString const& s,
                   QList<ExpectedStringGroup> const& x)
{
    TEST_CALL(testCrackVar, s, "%1", x, {{}}, {});
    TEST_CALL(testCrackVar, s, "%1.b", x, {{}}, {{".b"}});
    TEST_CALL(testCrackVar, s, "a%1", x, {{"a"}}, {});
    TEST_CALL(testCrackVar, s, "a%1.b", x, {{"a"}}, {{".b"}});
}

//-----------------------------------------------------------------------------
int testCracking(qtTest& t_obj)
{
    TEST_CALL(testCrack, "", {});
    TEST_CALL(testCrack, "a", {{"a"}});
    TEST_CALL(testCrack, "aa.b", {{"aa.b"}});

    TEST_CALL(testCrackVars, "0", {{"", 1, 0}});
    TEST_CALL(testCrackVars, "5", {{"5", 0, 1}});
    TEST_CALL(testCrackVars, "2x0", {{"2", 0, 1}, {"x"}, {"", 1, 0}});
    TEST_CALL(testCrackVars, "2x5", {{"2", 0, 1}, {"x"}, {"5", 0, 1}});
    TEST_CALL(testCrackVars, "2x05", {{"2", 0, 1}, {"x"}, {"5", 1, 1}});
    TEST_CALL(testCrackVars, "2x50", {{"2", 0, 1}, {"x"}, {"50", 0, 2}});
    TEST_CALL(testCrackVars, "2x050", {{"2", 0, 1}, {"x"}, {"50", 1, 2}});
    TEST_CALL(testCrackVars, "02x050", {{"2", 1, 1}, {"x"}, {"50", 1, 2}});
    TEST_CALL(testCrackVars, special5, {{special5, 0, 1}});

    return 0;
}

//-----------------------------------------------------------------------------
void testCompVar(qtTest& t_obj, QString const& s1, QString const& s2,
                 bool x, QString const& v)
{
    static compare c;

    const auto& v1 = v.arg(s1);
    const auto& v2 = v.arg(s2);

    TEST_EQUAL(c(v1, v1), false);
    TEST_EQUAL(c(v2, v2), false);

    TEST_EQUAL(c(v2, v1), x);
    TEST_EQUAL(c(v1, v2), !x);
}

//-----------------------------------------------------------------------------
void testCompVars(
    qtTest& t_obj, QString const& s1, QString const& s2, bool x)
{
    TEST_CALL(testCompVar, s1, s2, x, "%1");
    TEST_CALL(testCompVar, s1, s2, x, "%1.b");
    TEST_CALL(testCompVar, s1, s2, x, "a%1");
    TEST_CALL(testCompVar, s1, s2, x, "a%1.b");
}

//-----------------------------------------------------------------------------
int testCompare(qtTest& t_obj)
{
#define BIGNUM "55555555555555555555555" // To test numbers larger than 2^64

    TEST_CALL(testCompVars, "5", "30", false);
    TEST_CALL(testCompVars, "05", "30", false);
    TEST_CALL(testCompVars, "005", "50", false);
    TEST_CALL(testCompVars, "050", "40", true);
    TEST_CALL(testCompVars, "050", "50", true);
    TEST_CALL(testCompVars, "050", "400", false);
    TEST_CALL(testCompVars, "050", "600", false);
    TEST_CALL(testCompVars, "05x05", "600", false);
    TEST_CALL(testCompVars, "05x05", "5x60", false);
    TEST_CALL(testCompVars, "05x05", "05x60", false);
    TEST_CALL(testCompVars, "05x05", "06xxx", false);
    TEST_CALL(testCompVars, "05x05", "06x05", false);
    TEST_CALL(testCompVars, BIGNUM "5", BIGNUM "4", true);
    TEST_CALL(testCompVars, BIGNUM "5", BIGNUM "6", false);
    TEST_CALL(testCompVars, "a5", "5", true);
    TEST_CALL(testCompVars, "5", "5a", false);
    TEST_CALL(testCompVars, "q", "l", true);
    TEST_CALL(testCompVars, "4", special5, false);
    TEST_CALL(testCompVars, "5", special5, false);
    TEST_CALL(testCompVars, "6", special5, true);

    return 0;
}

//-----------------------------------------------------------------------------
int main()
{
  qtTest t_obj;

  t_obj.runSuite("Cracking", testCracking);
  t_obj.runSuite("Comparisons", testCompare);
  return t_obj.result();
}
