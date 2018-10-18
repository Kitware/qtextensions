/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
