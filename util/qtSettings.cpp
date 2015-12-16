/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtSettings.h"
#include "qtSettingsImpl.h"

#include <QCoreApplication>
#include <QSettings>
#include <QHash>
#include <QSet>

#include "qtAbstractSetting.h"

QTE_IMPLEMENT_D_FUNC(qtSettings)

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSettingsPrivate

//-----------------------------------------------------------------------------
class qtSettingsPrivate
{
public:
  class Setting;

  ~qtSettingsPrivate();

  QSettings& store(qtSettings::Scope);

  QHash<qtSettings::Scope, QSettings*> stores;
  QHash<QString, qtAbstractSetting*> settings;
  QSet<QString> modifiedSettings;
  bool wasCommitted;
};

//-----------------------------------------------------------------------------
qtSettingsPrivate::~qtSettingsPrivate()
{
  qDeleteAll(this->settings.values());
  qDeleteAll(this->stores.values());
}

//-----------------------------------------------------------------------------
QSettings& qtSettingsPrivate::store(qtSettings::Scope s)
{
  if (!this->stores.contains(s))
    {
    QSettings::Scope ss = (s.testFlag(qtSettings::UserScope)
                           ? QSettings::UserScope
                           : QSettings::SystemScope);
    QString appName = (s.testFlag(qtSettings::ApplicationScope)
                       ? qApp->applicationName() : QString());
    QSettings* settings = new QSettings(ss, qApp->organizationName(), appName);
    this->stores.insert(s, settings);
    return *settings;
    }

  return *this->stores[s];
}

//END qtSettingsPrivate

///////////////////////////////////////////////////////////////////////////////

//BEGIN Setting

//-----------------------------------------------------------------------------
class qtSettingsPrivate::Setting : public qtAbstractSetting
{
public:
  Setting() {}
  Setting(qtSettings::Scope, const QString& key,
          const QVariant& defaultValue, const QSettings& store);

  virtual qtSettings::Scope scope() const;

protected:
  virtual QString key() const;

  qtSettings::Scope storeScope;
  QString storeKey;
};

//-----------------------------------------------------------------------------
qtSettingsPrivate::Setting::Setting(
  qtSettings::Scope s,
  const QString& k,
  const QVariant& defaultValue,
  const QSettings& store)
  : storeScope(s), storeKey(k)
{
  this->originalValue = store.value(k, defaultValue);
  this->initialize(store);
}

//-----------------------------------------------------------------------------
qtSettings::Scope qtSettingsPrivate::Setting::scope() const
{
  return this->storeScope;
}

//-----------------------------------------------------------------------------
QString qtSettingsPrivate::Setting::key() const
{
  return this->storeKey;
}

//END Setting

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSettings

//-----------------------------------------------------------------------------
qtSettings::qtSettings() : d_ptr(new qtSettingsPrivate)
{
}

//-----------------------------------------------------------------------------
qtSettings::~qtSettings()
{
  QTE_D(qtSettings);
  delete d;
}

//-----------------------------------------------------------------------------
bool qtSettings::hasUncommittedChanges() const
{
  QTE_D_CONST(qtSettings);
  return !d->modifiedSettings.isEmpty();
}

//-----------------------------------------------------------------------------
bool qtSettings::wasCommitted() const
{
  QTE_D_CONST(qtSettings);
  return d->wasCommitted;
}

//-----------------------------------------------------------------------------
void qtSettings::commit()
{
  if (!this->hasUncommittedChanges())
    {
    return;
    }

  QTE_D(qtSettings);

  QSet<qtSettings::Scope> modifiedScopes;
  foreach (auto const& key, d->modifiedSettings.values())
    {
    qtAbstractSetting* s = d->settings[key];
    qtSettings::Scope scope = s->scope();
    s->commit(d->store(scope));
    modifiedScopes.insert(scope);
    }

  foreach (auto const s, modifiedScopes)
    d->store(s).sync();

  d->modifiedSettings.clear();
  d->wasCommitted = true;
}

//-----------------------------------------------------------------------------
void qtSettings::discard()
{
  QTE_D(qtSettings);

  foreach (auto const& key, d->modifiedSettings.values())
    d->settings[key]->discard();

  d->modifiedSettings.clear();
}

//-----------------------------------------------------------------------------
void qtSettings::clear()
{
  QTE_D(qtSettings);

  d->settings.clear();
  d->modifiedSettings.clear();

  foreach (auto const settings, d->stores.values())
    settings->clear();
}

//-----------------------------------------------------------------------------
void qtSettings::declareSetting(
  const QString& key, const QVariant& defaultValue, Scope scope)
{
  QTE_D(qtSettings);
  QSettings& store = d->store(scope);
  d->settings.insert(
    key, new qtSettingsPrivate::Setting(scope, key, defaultValue, store));
}

//-----------------------------------------------------------------------------
void qtSettings::declareSetting(const QString& key, qtAbstractSetting* s)
{
  QTE_D(qtSettings);
  QSettings& store = d->store(s->scope());
  s->initialize(store);
  d->settings.insert(key, s);
}

//-----------------------------------------------------------------------------
QVariant qtSettings::value(const QString& key) const
{
  QTE_D_CONST(qtSettings);
  if (!d->settings.contains(key))
    {
    return QVariant();
    }
  return d->settings[key]->value();
}

//-----------------------------------------------------------------------------
void qtSettings::setValue(const QString& key, const QVariant& value)
{
  QTE_D(qtSettings);

  if (!d->settings.contains(key))
    {
    return;
    }

  qtAbstractSetting* s = d->settings[key];
  s->setValue(value);

  if (s->isModified())
    {
    d->modifiedSettings.insert(key);
    }
  else
    {
    d->modifiedSettings.remove(key);
    }
}

//END qtSettings
