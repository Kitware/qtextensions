/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtScopedSettingsGroup.h"

#include <QSettings>

//-----------------------------------------------------------------------------
qtScopedSettingsGroup::qtScopedSettingsGroup(QSettings& s, const QString& g)
  : store(s)
{
  s.beginGroup(g);
}

//-----------------------------------------------------------------------------
qtScopedSettingsGroup::~qtScopedSettingsGroup()
{
  this->store.endGroup();
}
