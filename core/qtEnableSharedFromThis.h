/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtEnableSharedFromThis_h
#define __qtEnableSharedFromThis_h

#include <QEnableSharedFromThis>

template <typename Self, typename Object>
class qtEnableSharedFromThis
    : public Object, protected QEnableSharedFromThis<Self>
{
    friend class QSharedPointer<Self>;
};

#endif
