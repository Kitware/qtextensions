/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtSettingsImpl_h
#define __qtSettingsImpl_h

#include "qtSettings.h"

#define qtSettings_implement(_type_, _name_, _Name_) \
  _type_ QTSETTINGS_SUBCLASS_NAME::_name_() const { \
    return this->value(key##_Name_).value<_type_>(); \
  } \
  void QTSETTINGS_SUBCLASS_NAME::set##_Name_(_type_ value) { \
    this->setValue(key##_Name_, QVariant::fromValue(value)); \
  }

#endif
