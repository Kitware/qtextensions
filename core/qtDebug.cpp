/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtDebugImpl.h"

#include <QSettings>

#undef qtDebug

const qtDebugAreaAccessor qtDebug::InvalidArea = 0;

//-----------------------------------------------------------------------------
class qtDebugAreaPrivate
{
public:
  qtDebugAreaPrivate(const char* name, bool defaultActive);

  QString Name;
  bool Active;
};

//-----------------------------------------------------------------------------
qtDebugAreaPrivate::qtDebugAreaPrivate(const char* name, bool defaultActive) :
  Name(name)
{
  // Get user active value
  QSettings settings;
  settings.beginGroup("Debug");
  this->Active = settings.value(this->Name, defaultActive).toBool();
}

//-----------------------------------------------------------------------------
qtDebugArea::qtDebugArea(const char* name, bool defaultActive) :
  d_ptr(new qtDebugAreaPrivate(name, defaultActive))
{
}

//-----------------------------------------------------------------------------
qtDebugArea::~qtDebugArea()
{
  delete this->d_ptr;
}

//-----------------------------------------------------------------------------
qtDebug::qtDebug(qtDebugAreaAccessor areaAccessor)
{
  if (areaAccessor)
    {
    const qtDebugAreaPrivate* area = (*areaAccessor)()->d_ptr;
    if (area->Active)
      {
      this->d = QSharedPointer<QDebug>(new QDebug(QtDebugMsg));
      QString header = "qtDebug(%1):";
      (*this->d) << qPrintable(header.arg(area->Name));
      }
    }
}

//-----------------------------------------------------------------------------
bool qtDebug::isAreaActive(qtDebugAreaAccessor areaAccessor)
{
  if (areaAccessor)
    {
    const qtDebugAreaPrivate* area = (*areaAccessor)()->d_ptr;
    return area->Active;
    }
  return false;
}
