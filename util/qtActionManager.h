/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
    QString settingsKey, QString text,
    QString displayGroup = QString(), QString icon = QString(),
    QKeySequence defaultShortcut = QKeySequence());
  qtActionFactory* registerAction(
    QString settingsKey, QString text, QString displayGroup, QString icon,
    QList<QKeySequence> defaultShortcuts);

  void setupAction(QSettings& settings, QAction* action, QString settingsKey,
                   QString icon = QString());

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
  Q_DISABLE_COPY(qtActionManager)

  explicit qtActionManager(QObject* parent);
};

#define qtAm (qtActionManager::instance())

#endif
