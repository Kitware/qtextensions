/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtKstParser.h"

#include <QRegExp>

#include <cerrno>
#include <cmath>
#include <cstdlib>

#include "../core/qtMath.h"

// Don't use these; they assert on MSVC when fed non-ASCII characters
#define isspace { ERROR: isspace is broken on MSVC }
#define isdigit { ERROR: isdigit is broken on MSVC }

#ifdef Q_OS_WIN
  #define strtoll _strtoi64
#endif

typedef const char* regex_pattern;

#define XD "[\\dabcdef]"

static regex_pattern
reCommonNumber  = "([+-])?"                 // [1] significand sign
                  "(\\d+)"                  // [2] significand integer part
                  "([.,](\\d*))?"           // [4] significand fractional part
                  "(e([+-]?\\d+))?"         // [6] exponent
                  "(b(\\d+))?",             // [8] base value

reHexNumber     = "([+-])?"                 // [1] significand sign
                  "0x(" XD "+)"             // [2] significand integer part
                  "([.,](" XD "*))?"        // [4] significand fractional part
                  "([p^]([+-]?" XD "+))?",  // [6] exponent

reExtBaseNumber = "([+-])?"                 // [1] significand sign
                  "(\\w+)"                  // [2] significand integer part
                  "([.,](\\w*))?"           // [4] significand fractional part
                  "(\\^([+-]?\\w+))?"       // [6] exponent
                  "(@(\\d+))?",             // [8] base value

reArcNumberS    = "([+-])?"                 // [1] sign
                  "((\\d+)"                 // [3] degrees
                  "[d\xc2\xb0])?"           //     degree marker
                  "((\\d+)"                 // [5] minutes
                  "['\xe2\x80\xb2])?"       //     minute marker
                  "((\\d+)"                 // [7] seconds integer part
                  "([.,](\\d*))?"           // [9] seconds fractional part
                  "[\"\xe2\x80\xb3])?",     //     second marker

reArcNumberD    = "((\\d+)"                 // [2] degrees
                  "[d\xc2\xb0])?"           //     degree marker
                  "((\\d+)"                 // [4] minutes
                  "['\xe2\x80\xb2])?"       //     minute marker
                  "((\\d+)"                 // [6] seconds integer part
                  "([.,](\\d*))?"           // [8] seconds fractional part
                  "[\"\xe2\x80\xb3])?"      //     second marker
                  "([nsew])";               // [9] direction

//-----------------------------------------------------------------------------
double infinity(bool negative)
{
  return (negative ? -qInf() : qInf());
}

//-----------------------------------------------------------------------------
double zero(bool negative)
{
  return 1.0 / infinity(negative);
}

//-----------------------------------------------------------------------------
static bool digitsValid(const QString& str, int base = 10)
{
  bool okay;

  int n = str.length();
  while (n--)
    {
    str.mid(n, 1).toInt(&okay, base);
    if (!okay)
      {
      return false;
      }
    }
  return true;
}

//-----------------------------------------------------------------------------
static bool parseBasicLong(qint64& out, const QString& in)
{
  const QByteArray inAscii = in.toAscii();

  const int k = inAscii.length();
  if (k < 1 || !in[0].isDigit())
    {
    return false;
    }

  char* end;
  errno = 0;
  out = strtoll(inAscii.constData(), &end, 10);
  return (*end == 0 && errno == 0);
}

//-----------------------------------------------------------------------------
static bool parseLong(qint64& out, const QString& ss, QString sip, QString sfp,
                      const QString& exp, const QString& b, int bmax = 36)
{
  bool okay;

  // Read base, if any
  int base = 10;
  if (!b.isEmpty())
    {
    base = b.toInt(&okay);
    if (!okay || base > bmax || base < 2)
      {
      return false;
      }
    }

  // Read exponent, if any
  qint64 exponent = 0;
  if (!exp.isEmpty())
    {
    exponent = exp.toLongLong(&okay, base);
    if (!okay)
      {
      return false;
      }
    }
  // If an exponent is present, shift digits between the integer part and
  // fractional part accordingly
  while (exponent > 0)
    {
    if (sfp.isEmpty())
      {
      sip += '0';
      }
    else
      {
      sip += sfp.left(1);
      sfp = sfp.mid(1);
      }
    --exponent;
    }
  while (exponent < 0)
    {
    if (sip.isEmpty())
      {
      if (digitsValid(sfp, base))
        {
        out = 0;
        return true;
        }
      return false;
      }
    sfp = sip.right(1) + sfp;
    sip = sip.left(sip.length() - 1);
    ++exponent;
    }

  // Okay, we have the normalized (exponent == 0) integer part; discard the
  // fractional part, add the sign, and convert
  if (!digitsValid(sfp, base))
    {
    return false;
    }
  if (sip.isEmpty())
    {
    out = 0;
    return true;
    }
  sip = ss + sip;
  out = sip.toLongLong(&okay, base);
  return okay;
}

//-----------------------------------------------------------------------------
static bool parseBasicReal(double& out, const QString& in)
{
  const int k = in.length();
  if (k < 1)
    {
    return false;
    }

  double result = 0.0;
  double sign = 1.0;
  double exp = 1.0;
  int pos = 0;

  // Test for sign
  if (in[pos] == '-')
    {
    sign = -1.0;
    ++pos; // skip sign
    }
  else if (in[pos] == '+')
    {
    ++pos; // skip sign
    }

  // Read integer part
  while (pos < k)
    {
    const QChar d = in[pos];
    if (!d.isDigit())
      {
      break;
      }

    result = 10.0 * result + d.digitValue();
    ++pos;
    }

  // Check for decimal part
  if (pos < k)
    {
    const QChar n = in[pos];
    if (n == '.' || n == ',')
      {
      ++pos;

      // Read fractional part
      int dp = -1;
      while (pos < k)
        {
        const QChar d = in[pos];
        if (!d.isDigit())
          {
          break;
          }

        result += pow(10.0, dp) * d.digitValue();
        ++pos;
        --dp;
        }
      }

    // Check for exponent
    if (pos < k && result != 0.0 && in[pos].toLower() == 'e')
      {
      // Parse exponent part
      qint64 e;
      if (!parseBasicLong(e, in.mid(pos + 1)))
        {
        return false;
        }
      exp = pow(10.0, static_cast<double>(e));
      }
    }

  // Either we're done, or ran into something we can't parse
  if (pos < k)
    {
    return false;
    }

  // Success
  out = sign * result * exp;
  return true;
}

//-----------------------------------------------------------------------------
static bool parseReal(double& out, const QString& ss, QString sip, QString sfp,
                      const QString& exp, const QString& b, int bmax = 36)
{
  bool okay;

  // Read base, if any
  int base = 10;
  if (!b.isEmpty())
    {
    base = b.toInt(&okay);
    if (!okay || base > bmax || base < 2)
      {
      return false;
      }
    }

  // Evaluate the sign
  bool negative = (ss == "-");

  // Read exponent, if any
  qint64 exponent = 0;
  if (!exp.isEmpty())
    {
    exponent = exp.toLongLong(&okay, base);
    if (!okay)
      {
      if (digitsValid(exp, base) &&
          digitsValid(sip, base) && digitsValid(sfp, base))
        {
        // If the number is all valid digits, but the exponent is unreasonably
        // large or small, then return our respective overflow or underflow
        // value, i.e. +/- infinity or +/- 0.0
        if (exp.startsWith('-'))
          {
          out = zero(negative);
          }
        else
          {
          out = infinity(negative);
          }
        return true;
        }
      return false;
      }
    }

  // Calculate a cutoff; up to this value we will shift around digits and
  // adjust the exponent so that the accumulator is always an integer, to
  // minimize rounding error
  quint64 cutoff = (1LL << 48) / base;
  long double macc = 0.0;

  while (!sip.isEmpty() || !sfp.isEmpty())
    {
    long double acc = 0.0;
    while (!sip.isEmpty() || !sfp.isEmpty())
      {
      // If the integer part runs out of digits, swipe the first one from the
      // fractional part and adjust the exponent
      if (sip.isEmpty())
        {
        sip = sfp.left(1);
        sfp = sfp.mid(1);
        exponent--;
        }
      // Accumulate next digit
      int d = sip.left(1).toInt(&okay, base);
      sip = sip.mid(1);
      if (!okay)
        {
        return false;
        }
      acc = (acc * base) + d;
      if (acc > cutoff)
        {
        break;
        }
      }

    // At this point, we are either out of digits, or the remaining digits will
    // start losing precision... in either case apply the exponent to what we
    // have so far
    long double ev = pow(static_cast<long double>(base),
                         static_cast<long double>(exponent));
    acc *= ev;

    if (macc == 0.0)
      {
      // If this is the first pass, reduce the cutoff so we process only a
      // handful more digits, then bail since we won't be contributing
      // meaningfully to the result after that
      cutoff = 1 << 24;
      macc = acc;
      }
    else
      {
      // After the second pass, add the next grouping to the previous value and
      // bail, since whatever is left isn't going to contribute meaningfully to
      // the result
      macc += acc;
      if (!(digitsValid(sip, base) && digitsValid(sfp, base)))
        {
        return false;
        }
      break;
      }
    }

  out = (negative ? -macc : macc);
  return true;
}

//-----------------------------------------------------------------------------
static bool parseArcLong(qint64& out, const QString& sign,
                         QString deg, const QString& min,
                         const QString& sip, const QString& sfp)
{
  bool okay;

  if (!digitsValid(sfp))
    {
    return false;
    }
  if (deg.isEmpty() && min.isEmpty() && sip.isEmpty() && sfp.isEmpty())
    {
    return false;
    }

  out = 0;
  if (!deg.isEmpty())
    {
    out += deg.toLongLong(&okay);
    if (!okay)
      {
      return false;
      }
    }

  if (!min.isEmpty())
    {
    quint64 acc = min.left(min.length() - 1).toLongLong(&okay) / 6;
    if (!okay)
      {
      return false;
      }
    out += static_cast<qint64>(acc);
    if (out < 0)
      {
      return false;
      }
    }

  if (!sip.isEmpty())
    {
    quint64 acc = sip.left(sip.length() - 2).toLongLong(&okay) / 36;
    if (!okay)
      {
      return false;
      }
    out += static_cast<qint64>(acc);
    if (out < 0)
      {
      return false;
      }
    }

  if (QRegExp("[-sw]").exactMatch(sign))
    {
    out = -out;
    }
  return true;
}

//-----------------------------------------------------------------------------
static bool parseArcReal(double& out, const QString& sign,
                         const QString& deg, const QString& min,
                         const QString& sip, const QString& sfp)
{
  bool okay;

  if (deg.isEmpty() && min.isEmpty() && sip.isEmpty() && sfp.isEmpty())
    {
    return false;
    }

  out = 0;
  if (!deg.isEmpty())
    {
    out += deg.toDouble(&okay);
    if (!okay)
      {
      return false;
      }
    }

  if (!min.isEmpty())
    {
    out += min.toDouble(&okay) / 60.0;
    if (!okay)
      {
      return false;
      }
    }

  if (!(sip.isEmpty() && sfp.isEmpty()))
    {
    QString sec = sip + '.' + sfp;
    out += sec.toDouble(&okay) / 3600.0;
    if (!okay)
      {
      return false;
      }
    }

  if (QRegExp("[-sw]").exactMatch(sign))
    {
    out = -out;
    }

  return true;
}

//-----------------------------------------------------------------------------
#define PARSE_NUMBER(_pat, _func, ...) do { \
  QRegExp re(QString::fromUtf8(_pat)); \
  if (re.exactMatch(str)) \
    return _func(out, __VA_ARGS__); \
  } while (0)

//-----------------------------------------------------------------------------
bool qtKstParser::parseLong(const QString& str, qint64& out)
{
  // Handle trivial case
  if (parseBasicLong(out, str))
    {
    return true;
    }

  // Handle common, hexadecimal and high-base numbers
  PARSE_NUMBER(reCommonNumber, parseLong,
               re.cap(1), re.cap(2), re.cap(4), re.cap(6), re.cap(8), 10);
  PARSE_NUMBER(reHexNumber, parseLong,
               re.cap(1), re.cap(2), re.cap(4), re.cap(6), "16");
  PARSE_NUMBER(reExtBaseNumber, parseLong,
               re.cap(1), re.cap(2), re.cap(4), re.cap(6), re.cap(8));

  // Handle arc-length numbers
  PARSE_NUMBER(reArcNumberS, parseArcLong,
               re.cap(1), re.cap(3), re.cap(5), re.cap(7), re.cap(9));
  PARSE_NUMBER(reArcNumberD, parseArcLong,
               re.cap(9), re.cap(2), re.cap(4), re.cap(6), re.cap(8));

  // No match
  return false;
}

//-----------------------------------------------------------------------------
bool qtKstParser::parseReal(const QString& str, double& out)
{
  // Handle trivial case
  if (parseBasicReal(out, str))
    {
    return true;
    }

  QRegExp re;

  // Handle common, hexadecimal and high-base numbers
  PARSE_NUMBER(reCommonNumber, parseReal,
               re.cap(1), re.cap(2), re.cap(4), re.cap(6), re.cap(8), 10);
  PARSE_NUMBER(reHexNumber, parseReal,
               re.cap(1), re.cap(2), re.cap(4), re.cap(6), "16");
  PARSE_NUMBER(reExtBaseNumber, parseReal,
               re.cap(1), re.cap(2), re.cap(4), re.cap(6), re.cap(8));

  // Handle arc-length numbers
  PARSE_NUMBER(reArcNumberS, parseArcReal,
               re.cap(1), re.cap(3), re.cap(5), re.cap(7), re.cap(9));
  PARSE_NUMBER(reArcNumberD, parseArcReal,
               re.cap(9), re.cap(2), re.cap(4), re.cap(6), re.cap(8));

  // No match
  return false;
}
