// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
