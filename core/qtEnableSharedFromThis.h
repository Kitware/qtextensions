/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtEnableSharedFromThis_h
#define __qtEnableSharedFromThis_h

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)

#include <QEnableSharedFromThis>

template <typename Self, typename Object>
class qtEnableSharedFromThis
    : public Object, protected QEnableSharedFromThis<Self>
{
    friend class QSharedPointer<Self>;
};

#else

#include <QObject>
#include <QSharedPointer>
#include <QWeakPointer>

#include <type_traits>

template <typename Self, typename Object>
class qtEnableSharedFromThis : public Object
{
    static_assert(
        std::is_base_of<QObject, Object>::value,
        "Object must be QObject or a descendant of QObject");

protected:
    QSharedPointer<Self> sharedFromThis()
    {
        return qSharedPointerCast<Self>(QWeakPointer<Object>{this});
    }
};

#endif

#endif
