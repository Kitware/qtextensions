/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtSettings_h
#define __qtSettings_h

#include <QString>
#include <QVariant>

#include "../core/qtGlobal.h"

#define qtSettings_declare(_type_, _get_, _set_) \
  _type_ _get_() const; void _set_(_type_)

class qtAbstractSetting;

class qtSettingsPrivate;

class QTE_EXPORT qtSettings
{
public:
  enum ScopeFlag
    {
    UserScope           = 0x1,
    ApplicationScope    = 0x2,
    OrganizationScope   = 0x0,
    SystemScope         = 0x0,
    DefaultScope        = UserScope | ApplicationScope
    };
  Q_DECLARE_FLAGS(Scope, ScopeFlag)

  qtSettings();
  virtual ~qtSettings();

  bool hasUncommittedChanges() const;
  bool wasCommitted() const;

  void commit();
  void discard();
  void clear();

protected:
  QTE_DECLARE_PRIVATE_PTR(qtSettings)

  void declareSetting(const QString& key,
                      const QVariant& defaultValue = QVariant(),
                      Scope = DefaultScope);
  void declareSetting(const QString& key, qtAbstractSetting*);
  QVariant value(const QString& key) const;
  void setValue(const QString& key, const QVariant& value);

private:
  QTE_DECLARE_PRIVATE(qtSettings)
  QTE_DISABLE_COPY(qtSettings)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(qtSettings::Scope)

#endif
