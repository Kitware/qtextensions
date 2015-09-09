/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
