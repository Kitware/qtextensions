/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
