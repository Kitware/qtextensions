// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtRand_h
#define __qtRand_h

/// \file

#include <QtGlobal>

#include <cmath>
#include <cstdlib>

/// Return a pseudo-random number in the range [0.0, 1.0).
///
/// This function internally computes qrand()<code> / (RAND_MAX+1)</code> and
/// will have corresponding distribution characteristics and quality of
/// randomness. Similarly, it is affected by qsrand().
///
/// \sa qrand()
inline double qtRandD()
{
  static const double divisor = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
  return static_cast<double>(qrand()) * divisor;
}

/// Return a pseudo-random number in the range [0, \p max).
///
/// This function computes a pseudo-random number using qtRandD() mapped to the
/// desired range, yielding similar distribution characteristics and quality of
/// randomness (which may differ from using the modulo operator to perform
/// range reduction). The result is less than \p max. This function is affected
/// by qsrand().
inline int qtRand(int max)
{
  return static_cast<int>(floor(qtRandD() * max));
}

/// Return a pseudo-random number in the range [\p min, \p max).
/// \copydetails qtRand(int)
///
/// If \p max is less than \p min, the result is undefined.
inline int qtRand(int min, int max)
{
  return min + static_cast<int>(floor(qtRandD() * (max - min)));
}

#endif
