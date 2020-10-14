// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtOnce_h
#define __qtOnce_h

/// \file

#include <QtGlobal> // for platform test symbols

#include "qtGlobal.h"

#ifdef Q_OS_WIN

#include <windows.h>

struct QTE_EXPORT qtOnceGuard
{
  volatile LONG init;
  volatile PVOID mutex;
};

#define QTE_ONCE_INIT { 0, 0 }

#else

#include <pthread.h>

typedef pthread_once_t qtOnceGuard;

#define QTE_ONCE_INIT PTHREAD_ONCE_INIT

#endif

/// Call a function exactly once.
///
/// qtOnce is a thread-safe function that can be used to perform one-time
/// initialization. It is guaranteed that \p func will be called exactly once
/// for the same \p guard, regardless of how many times, or by how many threads
/// (concurrently), qtOnce is called. On return, it is guaranteed that \p func
/// has executed to completion in some thread (not necessarily the calling
/// thread). \p func must not throw.
///
/// \note On POSIX platforms, this is a wrapper around pthread_once. On
///       Windows, a custom implementation is provided that implements FIFO
///       behavior (that is, concurrent calls to qtOnce will return in the same
///       order in which they are dispatched, and the first caller will execute
///       \p func).
///
/// \sa QTE_ONCE
QTE_EXPORT void qtOnce(qtOnceGuard& guard, void(*func)(void));

/// Declare guard for qtOnce.
///
/// This declares a guard named \p name for qtOnce. The macro takes care of
/// declaring the guard as the proper type and providing an appropriate static
/// initializer for the guard.
#define QTE_ONCE(name) qtOnceGuard name = QTE_ONCE_INIT

#endif
