/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtColorScheme_h
#define __qtColorScheme_h

#include <QPalette>

#include "../core/qtGlobal.h"

class QSettings;

class QTE_EXPORT qtColorScheme : public QPalette
{
public:
  qtColorScheme();
  qtColorScheme(const QPalette& other) : QPalette(other) {}

  qtColorScheme& operator=(const QPalette& other)
  { this->QPalette::operator=(other); return *this; }

  void load(const QSettings&);
  void save(QSettings&) const;

  static qtColorScheme fromSettings(const QSettings&);

protected:
  void load(QPalette::ColorGroup, const QString& group, const QSettings&);
  void save(QPalette::ColorGroup, const QString& group, QSettings&) const;

  void load(QPalette::ColorGroup, const QString& group,
            QPalette::ColorRole, const QString& role,
            const QSettings&);

  void save(QPalette::ColorGroup, const QString& group,
            QPalette::ColorRole, const QString& role,
            QSettings&) const;

  QString key(const QString& group, const QString& role) const;
};

#endif
