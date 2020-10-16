// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtNaturalSort.h"

#include <QVector>

namespace qtNaturalSort
{

namespace // anonymous
{

//-----------------------------------------------------------------------------
struct Char
{
    uint codePoint;
    short units;

    bool isDigit() const
    { return QChar::category(codePoint) == QChar::Number_DecimalDigit; }

    int digitValue() const
    { return QChar::digitValue(codePoint); }
};

//-----------------------------------------------------------------------------
Char getChar(QString const& s, int pos)
{
    auto const& c1 = s.at(pos);
    if (c1.isHighSurrogate() && pos + 1 < s.length())
    {
        auto const& c2 = s.at(pos + 1);
        auto const b1 = c1.unicode() - 0xd800u;
        auto const b2 = c2.unicode() - 0xdc00u;
        return {0x10000u + (b1 << 10) + b2, 2};
    }

    return {c1.unicode(), 1};
}

//-----------------------------------------------------------------------------
Char getChar(QStringRef const& s, int pos)
{
    auto const& c1 = s.at(pos);
    if (c1.isHighSurrogate() && pos + 1 < s.length())
    {
        auto const& c2 = s.at(pos + 1);
        auto const b1 = c1.unicode() - 0xd800u;
        auto const b2 = c2.unicode() - 0xdc00u;
        return {0x10000u + (b1 << 10) + b2, 2};
    }

    return {c1.unicode(), 1};
}

//-----------------------------------------------------------------------------
int compareNumbers(StringGroup const& g1, StringGroup const& g2)
{
    Q_ASSERT(g1.significantDigits == g2.significantDigits);

    auto const n1 = g1.characters.length();
    auto const n2 = g2.characters.length();
    decltype(n1 + 0) i1 = 0;
    decltype(n2 + 0) i2 = 0;

    while (i1 < n1 && i2 < n2)
    {
        auto const& c1 = getChar(g1.characters, i1);
        auto const& c2 = getChar(g2.characters, i2);

        if (auto const d = (c1.digitValue() - c2.digitValue()))
            return d;

        i1 += c1.units;
        i2 += c2.units;
    }

    return 0;
}

} // namespace <anonymous>

//-----------------------------------------------------------------------------
QVector<StringGroup> crack(QString const& s)
{
    QVector<StringGroup> out;

    auto const n = s.length();
    decltype(n + 0) i = 0;
    decltype(n + 0) j = 0;

    // Iterate over string
    while (j < n)
    {
        // Stop at number groups
        auto c = getChar(s, j);
        if (c.isDigit())
        {
            // Append non-numeric group to output
            out.append({s.midRef(i, j - i), 0, 0});

            // Skip leading zeros
            i = j;
            while (j < n && c.digitValue() == 0)
            {
                j += c.units;
                if (j < n)
                {
                    c = getChar(s, j);
                    if (!c.isDigit())
                        break;
                }
            }
            auto const l = j - i;

            // Get numeric part
            int d = 0;
            i = j;
            while (j < n && (c = getChar(s, j)).isDigit())
            {
                ++d;
                j += c.units;
            }

            // Append numeric group to output
            out.append({s.midRef(i, j - i), l, d});
            i = j;
        }
        else
        {
            ++j;
        }
    }

    // Append final non-numeric group to output
    if (i < j)
        out.append({s.midRef(i), 0, 0});

    return out;
}

//-----------------------------------------------------------------------------
bool compare::operator()(QString const& s1, QString const& s2)
{
  auto const& p1 = crack(s1);
  auto const& p2 = crack(s2);

  auto const n1 = p1.count();
  auto const n2 = p2.count();
  auto const n = qMin(n1, n2);

  int ri = 0;

  for (decltype(n + 0) i = 0; i < n; ++i)
  {
    // Compare non-numeric group first; if they differ, return result
    if (auto const r = p1[i].characters.compare(p2[i].characters))
      return (r < 0);

    // Is there a following non-numeric group?
    if (++i >= n)
      break;

    // Compare numeric groups
    auto const& g1 = p1[i];
    auto const& g2 = p2[i];

    // If one number has more digits, it must be greater
    if (g1.significantDigits != g2.significantDigits)
      return (g1.significantDigits < g2.significantDigits);

    // Otherwise, compare numeric values
    if (auto r = compareNumbers(g1, g2))
      return (r < 0);

    // If numeric values are the same, but have different number of leading
    // zeros, make a note to prefer the one with fewer leading zeros if we
    // don't find any other differences
    ri = (ri ? ri : g1.leadingZeros - g2.leadingZeros);
  }

  // Check for a difference after one string's final numeric group
  if (n1 != n2)
    return (n1 < n2);

  // If one numeric group had more leading zeros, consider that one "greater"
  if (ri)
    return (ri < 0);

  // If all else fails, fall back to a straight lexicographical comparison
  // (e.g. two strings that differ only in a numeric group that has the same
  // value, but uses different types of digits)
  return (s1 < s2);
}

} // namespace qtNaturalSort
