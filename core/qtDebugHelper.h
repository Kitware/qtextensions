// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDebugHelper_h
#define __qtDebugHelper_h

#include <QDebug>

#define QDEBUG_ENUM_HANDLER_BEGIN(type) \
  QDebug& operator<<(QDebug& dbg, type _value) \
    { \
    if (0);

#define QDEBUG_HANDLE_ENUM(value) \
    else if (_value == value) dbg << #value;

#define QDEBUG_ENUM_HANDLER_END(type) \
    else \
      { \
      QString s(#type "::(unknown enum value %1)"); \
      dbg << qPrintable(s.arg(_value)); \
      } \
    return dbg; \
    }

#endif
