/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtScopedSettingGroup_h
#define __qtScopedSettingGroup_h

#include "../core/qtGlobal.h"

class QSettings;
class QString;

class QTE_EXPORT qtScopedSettingGroup
{
public:
  explicit qtScopedSettingGroup(QSettings&, const QString&);
  ~qtScopedSettingGroup();

protected:
  QSettings& store;
};

#endif
