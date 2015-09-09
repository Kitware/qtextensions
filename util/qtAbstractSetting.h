/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtAbstractSetting_h
#define __qtAbstractSetting_h

#include "qtSettings.h"

class QSettings;

class QTE_EXPORT qtAbstractSetting
{
public:
  qtAbstractSetting();
  virtual ~qtAbstractSetting();

  virtual qtSettings::Scope scope() const = 0;

  virtual bool isModified();
  virtual QVariant value() const;
  virtual void setValue(const QVariant& value);
  virtual void commit(QSettings& store);
  virtual void discard();

protected:
  friend class qtSettings;

  virtual void initialize(const QSettings& store);
  virtual QString key() const = 0;

  QVariant originalValue;
  QVariant currentValue;
  bool modified;
};

#endif
