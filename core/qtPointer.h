/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtPointer_h
#define __qtPointer_h

/// \file

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))

// Prefer QPointer over QWeakPointer in Qt5
// See https://www.macieira.org/blog/2012/07/continue-using-qpointer/
#include <QPointer>
template <class T>
using qtPointer = QPointer<T>;

#else

#include <QWeakPointer>
template <class T>
using qtPointer = QWeakPointer<T>;

#endif

#endif
