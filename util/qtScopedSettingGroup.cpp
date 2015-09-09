/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtScopedSettingGroup.h"

#include <QSettings>

//-----------------------------------------------------------------------------
qtScopedSettingGroup::qtScopedSettingGroup(QSettings& s, const QString& g)
  : store(s)
{
  s.beginGroup(g);
}

//-----------------------------------------------------------------------------
qtScopedSettingGroup::~qtScopedSettingGroup()
{
  this->store.endGroup();
}
