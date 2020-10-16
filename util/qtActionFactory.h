// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtActionFactory_h
#define __qtActionFactory_h

#include <QAction>
#include <QSettings>
#include <QString>

#include "../core/qtGlobal.h"

class qtActionFactoryPrivate;
class qtActionManagerDialog;

class QTE_EXPORT qtActionFactory
{
public:
  QAction* createAction(QObject* parent, QActionGroup* group = 0) const;
  QAction* createAction(QSettings&, QObject* parent,
                        QActionGroup* group = 0) const;

  void setShortcutContext(Qt::ShortcutContext);
  void setCheckable(bool);
  void setToolTip(const QString&);
  void setStatusTip(const QString&);
  void setWhatsThis(const QString&);
  void setPriority(QAction::Priority);

protected:
  QTE_DECLARE_PRIVATE(qtActionFactory)
  friend class qtActionManager;

  explicit qtActionFactory(QString settingsKey, QString text,
                           QString displayGroup, QString icon,
                           QList<QKeySequence> defaultShortcuts);

  ~qtActionFactory();

  void addToDialog(qtActionManagerDialog*) const;

private:
  QTE_DECLARE_PRIVATE_PTR(qtActionFactory)
  QTE_DISABLE_COPY(qtActionFactory)
};

#define qtAm (qtActionManager::instance())

#endif
