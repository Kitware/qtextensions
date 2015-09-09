/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
