/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtColorScheme.h"

#include <QSettings>

#define ARGS(x) QPalette::x, #x

//-----------------------------------------------------------------------------
qtColorScheme::qtColorScheme()
{
}

//-----------------------------------------------------------------------------
void qtColorScheme::load(const QSettings& settings)
{
  this->load(ARGS(Active), settings);
  this->load(ARGS(Inactive), settings);
  this->load(ARGS(Disabled), settings);
}

//-----------------------------------------------------------------------------
void qtColorScheme::load(QPalette::ColorGroup group, const QString& groupName,
                         QSettings const& settings)
{
  this->load(group, groupName, ARGS(Base), settings);
  this->load(group, groupName, ARGS(Text), settings);
  this->load(group, groupName, ARGS(AlternateBase), settings);
  this->load(group, groupName, ARGS(Window), settings);
  this->load(group, groupName, ARGS(WindowText), settings);
  this->load(group, groupName, ARGS(Button), settings);
  this->load(group, groupName, ARGS(ButtonText), settings);
  this->load(group, groupName, ARGS(Highlight), settings);
  this->load(group, groupName, ARGS(HighlightedText), settings);
  this->load(group, groupName, ARGS(ToolTipBase), settings);
  this->load(group, groupName, ARGS(ToolTipText), settings);
  this->load(group, groupName, ARGS(Link), settings);
  this->load(group, groupName, ARGS(LinkVisited), settings);
  this->load(group, groupName, ARGS(BrightText), settings);
}

//-----------------------------------------------------------------------------
void qtColorScheme::load(QPalette::ColorGroup group, const QString& groupName,
                         QPalette::ColorRole role, const QString& roleName,
                         const QSettings& settings)
{
  auto const& defaultColor = this->color(group, role);

  auto const& colorName = settings.value(
    this->key(groupName, roleName), defaultColor).toString();
  this->setColor(group, role, QColor{colorName});
}

//-----------------------------------------------------------------------------
void qtColorScheme::save(QSettings& settings) const
{
  this->save(ARGS(Active), settings);
  this->save(ARGS(Inactive), settings);
  this->save(ARGS(Disabled), settings);
}

//-----------------------------------------------------------------------------
void qtColorScheme::save(QPalette::ColorGroup group, const QString& groupName,
                         QSettings& settings) const
{
  this->save(group, groupName, ARGS(Base), settings);
  this->save(group, groupName, ARGS(Text), settings);
  this->save(group, groupName, ARGS(AlternateBase), settings);
  this->save(group, groupName, ARGS(Window), settings);
  this->save(group, groupName, ARGS(WindowText), settings);
  this->save(group, groupName, ARGS(Button), settings);
  this->save(group, groupName, ARGS(ButtonText), settings);
  this->save(group, groupName, ARGS(Highlight), settings);
  this->save(group, groupName, ARGS(HighlightedText), settings);
  this->save(group, groupName, ARGS(ToolTipBase), settings);
  this->save(group, groupName, ARGS(ToolTipText), settings);
  this->save(group, groupName, ARGS(Link), settings);
  this->save(group, groupName, ARGS(LinkVisited), settings);
  this->save(group, groupName, ARGS(BrightText), settings);
}

//-----------------------------------------------------------------------------
void qtColorScheme::save(QPalette::ColorGroup group, const QString& groupName,
                         QPalette::ColorRole role, const QString& roleName,
                         QSettings& settings) const
{
  settings.setValue(this->key(groupName, roleName),
                    this->color(group, role).name());
}

//-----------------------------------------------------------------------------
QString qtColorScheme::key(const QString& group, const QString& role) const
{
  return QString{"%1/%2"}.arg(group, role);
}

//-----------------------------------------------------------------------------
qtColorScheme qtColorScheme::fromSettings(const QSettings& settings)
{
  qtColorScheme scheme;
  scheme.load(settings);
  return scheme;
}
