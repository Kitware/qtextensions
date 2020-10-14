// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
