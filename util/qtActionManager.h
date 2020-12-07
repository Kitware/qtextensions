// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtActionManager_h
#define __qtActionManager_h

#include <QAction>
#include <QSettings>
#include <QString>

#include "../core/qtGlobal.h"

class qtActionFactory;
class qtActionManagerDialog;
class qtActionManagerPrivate;

class QTE_EXPORT qtActionManager : public QObject
{
  Q_OBJECT

public:
  static qtActionManager* instance();

  virtual ~qtActionManager();

  qtActionFactory* registerAction(
    QString settingsKey, QString text, QString displayGroup = {},
    QString icon = {}, QKeySequence defaultShortcut = {});
  qtActionFactory* registerAction(
    QString settingsKey, QString text, QString displayGroup, QString icon,
    QList<QKeySequence> defaultShortcuts);

  void setupAction(QSettings& settings, QAction* action, QString settingsKey,
                   QString icon = {});

protected slots:
  void unregisterStaticAction(QObject*);

protected:
  QTE_DECLARE_PRIVATE(qtActionManager)
  friend class qtActionManagerDialog;

  void addDynamicActions(qtActionManagerDialog*) const;
  void reloadActions(QSettings&) const;

  static void loadShortcut(QAction*, QSettings&, QString settingsKey);

private:
  QTE_DECLARE_PRIVATE_PTR(qtActionManager)
  QTE_DISABLE_COPY(qtActionManager)

  explicit qtActionManager(QObject* parent);
};

#define qtAm (qtActionManager::instance())

#endif
