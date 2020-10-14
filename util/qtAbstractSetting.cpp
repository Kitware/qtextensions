// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtAbstractSetting.h"

#include <QSettings>

//-----------------------------------------------------------------------------
qtAbstractSetting::qtAbstractSetting() : modified(false)
{
}

//-----------------------------------------------------------------------------
qtAbstractSetting::~qtAbstractSetting()
{
}

//-----------------------------------------------------------------------------
void qtAbstractSetting::initialize(const QSettings&)
{
  this->currentValue = this->originalValue;
}

//-----------------------------------------------------------------------------
bool qtAbstractSetting::isModified()
{
  return this->modified;
}

//-----------------------------------------------------------------------------
QVariant qtAbstractSetting::value() const
{
  return this->currentValue;
}

//-----------------------------------------------------------------------------
void qtAbstractSetting::setValue(const QVariant& value)
{
  this->currentValue = value;
  this->modified = (this->currentValue != this->originalValue);
}

//-----------------------------------------------------------------------------
void qtAbstractSetting::commit(QSettings& store)
{
  store.setValue(this->key(), this->currentValue);
  this->originalValue = this->currentValue;
  this->modified = false;
}

//-----------------------------------------------------------------------------
void qtAbstractSetting::discard()
{
  this->currentValue = this->originalValue;
  this->modified = false;
}
