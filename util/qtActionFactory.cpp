/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtActionFactory.h"

#include "../dialogs/qtActionManagerDialog.h"

#include "qtActionManager.h"

QTE_IMPLEMENT_D_FUNC(qtActionFactory)

//-----------------------------------------------------------------------------
class qtActionFactoryPrivate
{
public:
  qtActionFactoryPrivate(QString settingsKey, QString text,
                         QString displayGroup, QString icon,
                         QList<QKeySequence> defaultShortcuts);

  QString settingsKey;
  QString text, displayGroup, icon;
  QList<QKeySequence> defaultShortcuts;

  Qt::ShortcutContext shortcutContext;
  bool checkable;
  QString toolTip, statusTip, whatsThis;
  QAction::Priority priority;
};

//-----------------------------------------------------------------------------
qtActionFactoryPrivate::qtActionFactoryPrivate(
  QString k, QString t, QString dg, QString i, QList<QKeySequence> ds)
  : settingsKey(k), text(t), displayGroup(dg), icon(i),
    defaultShortcuts(ds), shortcutContext(Qt::WindowShortcut),
    checkable(false), priority(QAction::NormalPriority)
{
}

//-----------------------------------------------------------------------------
qtActionFactory::qtActionFactory(QString settingsKey, QString text,
                                 QString displayGroup, QString icon,
                                 QList<QKeySequence> defaultShortcuts)
  : d_ptr(new qtActionFactoryPrivate(settingsKey, text, displayGroup, icon,
                                     defaultShortcuts))
{
}

//-----------------------------------------------------------------------------
qtActionFactory::~qtActionFactory()
{
  QTE_D(qtActionFactory);
  delete d;
}

//-----------------------------------------------------------------------------
QAction* qtActionFactory::createAction(QObject* parent,
                                       QActionGroup* group) const
{
  QSettings settings;
  return this->createAction(settings, parent, group);
}

//-----------------------------------------------------------------------------
QAction* qtActionFactory::createAction(QSettings& settings, QObject* parent,
                                       QActionGroup* group) const
{
  QTE_D_CONST(qtActionFactory);

  // Create action
  QAction* action = new QAction(d->text, parent);
  action->setShortcuts(d->defaultShortcuts);

  // Set managed properties
  qtAm->setupAction(settings, action, d->settingsKey, d->icon);

  // Set additional properties
  if (group)
    {
    action->setActionGroup(group);
    }
  action->setShortcutContext(d->shortcutContext);
  action->setCheckable(d->checkable);
  action->setToolTip(d->toolTip);
  action->setStatusTip(d->statusTip);
  action->setWhatsThis(d->whatsThis);
  action->setPriority(d->priority);

  // Return the newly created action
  return action;
}

//-----------------------------------------------------------------------------
void qtActionFactory::setShortcutContext(Qt::ShortcutContext shortcutContext)
{
  QTE_D(qtActionFactory);
  d->shortcutContext = shortcutContext;
}

//-----------------------------------------------------------------------------
void qtActionFactory::setCheckable(bool checkable)
{
  QTE_D(qtActionFactory);
  d->checkable = checkable;
}

//-----------------------------------------------------------------------------
void qtActionFactory::setToolTip(const QString& toolTip)
{
  QTE_D(qtActionFactory);
  d->toolTip = toolTip;
}

//-----------------------------------------------------------------------------
void qtActionFactory::setStatusTip(const QString& statusTip)
{
  QTE_D(qtActionFactory);
  d->statusTip = statusTip;
}

//-----------------------------------------------------------------------------
void qtActionFactory::setWhatsThis(const QString& whatsThis)
{
  QTE_D(qtActionFactory);
  d->whatsThis = whatsThis;
}

//-----------------------------------------------------------------------------
void qtActionFactory::setPriority(QAction::Priority priority)
{
  QTE_D(qtActionFactory);
  d->priority = priority;
}

//-----------------------------------------------------------------------------
void qtActionFactory::addToDialog(qtActionManagerDialog* dialog) const
{
  QTE_D_CONST(qtActionFactory);
  QString text = QString(d->text).remove('&');
  dialog->registerAction(d->settingsKey, d->displayGroup, text, d->icon,
                         d->defaultShortcuts);
}
