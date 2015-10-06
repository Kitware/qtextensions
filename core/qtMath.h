/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtMath_h
#define __qtMath_h

#include <cmath>

#include <QtCore>

#ifdef Q_OS_WIN

// Replacements for C99 standard math functions that are missing on Windows

inline double round(double a)
{ return floor(a + 0.5); }

inline double trunc(double a)
{ return (a < 0.0 ? ceil(a) : floor(a)); }

#endif

/// Return a value representing the sign of a real number.
///
/// This function will return a value that is loosely equivalent to
/// <code>a / abs(\p a)</code>; that is, 1.0 for positive numbers, or -1.0 for
/// negative numbers, including ±infinity. Numbers that are exactly zero will
/// be returned as-is (preserving the sign bit), as will NaN's. Small values,
/// including denormalized numbers, will return ±1.0.
inline float qtSign(float a)
{ return (qIsNaN(a) ? a : a > 0.0 ? 1.0 : a < 0.0 ? -1.0 : a); }

/// \copydoc qtSign(float)
inline double qtSign(double a)
{ return (qIsNaN(a) ? a : a > 0.0 ? 1.0 : a < 0.0 ? -1.0 : a); }

#endif
