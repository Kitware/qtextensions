/*ckwg +5
 * Copyright 2019 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtNaturalSort_h
#define __qtNaturalSort_h

#include <qtExports.h>

#include <QString>

template<typename T> class QVector;

namespace qtNaturalSort
{
    /// Natural string character group
    ///
    /// This structure describes a character group of a natural string. A
    /// character group consists of either digits or non-digits. A digit group
    /// has a number of (significant) digits and an optional number of leading
    /// zero characters, which are not included in \p characters.
    ///
    /// Note that a numeric group may consist of only zeros, in which case
    /// \p characters will be empty.
    struct StringGroup
    {
        QStringRef characters;
        int leadingZeros;
        int significantDigits;
    };

    /// Split a string into "natural" groups
    ///
    /// This function splits a string into zero or more groups of characters,
    /// strictly alternating between non-digit groups and digit groups. The
    /// first group is always non-digits, and will be empty if the input string
    /// starts with a digit; otherwise, non-digit groups will never be empty.
    /// Digit groups may be "empty" iff the group consists only of zeros.
    ///
    /// \return Vector of StringGroup instances.
    ///
    /// \warning
    ///   The groups returned by this function \em reference the original
    ///   string. Callers must take care to not use the returned vector after
    ///   the input string has been modified or destroyed.
    QTE_EXPORT QVector<StringGroup> crack(QString const& s);

    /// Functor used to compare two strings "naturally".
    ///
    /// This functor is used to perform a "natural" comparison of two strings.
    /// A "natural" comparison splits each string into sets of numeric and
    /// non-numeric parts, where "numeric" is defined as "a sequence of digits"
    /// (Unicode class \c Nd). Non-numeric parts are compared using traditional
    /// lexicographical ordering, while numeric parts are compared numerically.
    /// If all non-numeric portions of the string are identical, and all
    /// numeric portions are numerically identical, the first numeric pair with
    /// a differing number of leading zeros determines the order. Otherwise,
    /// pure lexicographical order is used.
    ///
    /// Alternate digit forms, including those which require multi-unit
    /// encoding in UTF-16, are handled properly. Additionally, numeric
    /// comparison is performed digit by digit, allowing a correct result to be
    /// produced in the face of arbitrarily long digit sequences.
    ///
    /// \note
    ///   General purpose natural sorting is an impossible problem. In order to
    ///   produce "correct" results in all cases, the algorithm would need to
    ///   know additional information such as how to recognize hexadecimal
    ///   numbers, when to consider a non-ISO date as day/month or month/day,
    ///   whether to treat \c "1.20" as a floating point number or a version
    ///   string (i.e. should \c "1.20" be sorted before or after \c "1.5"?),
    ///   and whether to treat <code>"1,500"<code> as one number, or two
    ///   numbers separated by punctuation. This algorithm makes no attempt to
    ///   handle dates, digit separators, non-integers, or hexadecimal numbers.
    ///   Non-decimal digits and digit separators are treated as non-numeric
    ///   components.
    ///
    /// \sa qtNaturalSort::crack, QChar::isDigit
    struct QTE_EXPORT compare
    {
        bool operator()(QString const& s1, QString const& s2);
    };
} // namespace qtNaturalSort

#endif
