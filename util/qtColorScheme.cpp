// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  this->load(group, groupName, ARGS(PlaceholderText), settings);
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
  this->load(group, groupName, ARGS(Light), settings);
  this->load(group, groupName, ARGS(Midlight), settings);
  this->load(group, groupName, ARGS(Mid), settings);
  this->load(group, groupName, ARGS(Dark), settings);
  this->load(group, groupName, ARGS(Shadow), settings);
}

//-----------------------------------------------------------------------------
void qtColorScheme::load(QPalette::ColorGroup group, const QString& groupName,
                         QPalette::ColorRole role, const QString& roleName,
                         const QSettings& settings)
{
  auto const& color = settings.value(this->key(groupName, roleName));
  if (color.isValid())
    {
    this->setColor(group, role, QColor{color.toString()});
    }
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
  this->save(group, groupName, ARGS(PlaceholderText), settings);
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
  this->save(group, groupName, ARGS(Light), settings);
  this->save(group, groupName, ARGS(Midlight), settings);
  this->save(group, groupName, ARGS(Mid), settings);
  this->save(group, groupName, ARGS(Dark), settings);
  this->save(group, groupName, ARGS(Shadow), settings);
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
