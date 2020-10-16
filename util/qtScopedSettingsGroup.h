// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtScopedSettingsGroup_h
#define __qtScopedSettingsGroup_h

#include "../core/qtGlobal.h"

class QSettings;
class QString;

class QTE_EXPORT qtScopedSettingsGroup
{
public:
  explicit qtScopedSettingsGroup(QSettings&, const QString&);
  ~qtScopedSettingsGroup();

protected:
  QSettings& store;
};

#endif
