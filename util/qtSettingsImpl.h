// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
