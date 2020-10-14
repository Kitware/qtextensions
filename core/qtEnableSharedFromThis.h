// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
