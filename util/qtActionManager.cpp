/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtActionManager.h"

#include <QCoreApplication>
#include <QHash>

#include "../core/qtEnumerate.h"
#include "../core/qtUtil.h"

#include "qtActionFactory.h"

QTE_IMPLEMENT_D_FUNC(qtActionManager)

namespace
{
typedef QHash<QAction*, QString> StaticActionMap;
}

//-----------------------------------------------------------------------------
class qtActionManagerPrivate
{
public:
  QList<qtActionFactory*> actions;
  StaticActionMap staticActions;
};

//-----------------------------------------------------------------------------
qtActionManager* qtActionManager::instance()
{
  static qtActionManager* theInstance = 0;

  // Create global instance, if it doesn't exist. The global QCoreApplication
  // instance must be created first, as we parent ourselves to it so that it
  // will garbage collect us on shutdown.
  if (!theInstance)
    {
    QCoreApplication* app = QCoreApplication::instance();
    if (app)
      {
      // Take this opportunity to register QKeySequence as a metatype
      qRegisterMetaType<QKeySequence>("QKeySequence");
      qRegisterMetaTypeStreamOperators<QKeySequence>("QKeySequence");
      theInstance = new qtActionManager(app);
      }
    }

  return theInstance;
}

//-----------------------------------------------------------------------------
qtActionManager::qtActionManager(QObject* parent)
  : QObject(parent), d_ptr(new qtActionManagerPrivate)
{
}

//-----------------------------------------------------------------------------
qtActionManager::~qtActionManager()
{
  QTE_D(qtActionManager);
  foreach (auto const f, d->actions)
    delete f;
  delete d;
}

//-----------------------------------------------------------------------------
qtActionFactory* qtActionManager::registerAction(
  QString settingsKey, QString text, QString displayGroup, QString icon,
  QKeySequence defaultShortcut)
{
  QList<QKeySequence> defaultShortcuts;
  defaultShortcuts.append(defaultShortcut);
  return this->registerAction(settingsKey, text, displayGroup, icon,
                              defaultShortcuts);
}

//-----------------------------------------------------------------------------
qtActionFactory* qtActionManager::registerAction(
  QString settingsKey, QString text, QString displayGroup, QString icon,
  QList<QKeySequence> defaultShortcuts)
{
  if (settingsKey.isEmpty())
    {
    return 0;
    }

  if (displayGroup.isEmpty())
    {
    displayGroup = qApp->applicationName();
    }

  QTE_D(qtActionManager);
  qtActionFactory* f = new qtActionFactory(settingsKey, text, displayGroup,
                                           icon, defaultShortcuts);
  d->actions.append(f);
  return f;
}

//-----------------------------------------------------------------------------
void qtActionManager::setupAction(QSettings& settings, QAction* action,
                                  QString settingsKey, QString icon)
{
  QTE_D(qtActionManager);

  // Set shortcut from settings
  this->loadShortcut(action, settings, settingsKey);

  // Set up proper icon
  if (!icon.isEmpty())
    {
    action->setIcon(qtUtil::standardActionIcon(icon));
    }

  // Register static action so we can automatically reload its shortcut
  d->staticActions.insert(action, settingsKey);
  connect(action, SIGNAL(destroyed(QObject*)),
          this, SLOT(unregisterStaticAction(QObject*)));
}

//-----------------------------------------------------------------------------
void qtActionManager::unregisterStaticAction(QObject* object)
{
  QTE_D(qtActionManager);
  d->staticActions.remove(qobject_cast<QAction*>(object));
}

//-----------------------------------------------------------------------------
void qtActionManager::addDynamicActions(qtActionManagerDialog* dialog) const
{
  QTE_D_CONST(qtActionManager);
  foreach (auto const f, d->actions)
    f->addToDialog(dialog);
}

//-----------------------------------------------------------------------------
void qtActionManager::reloadActions(QSettings& settings) const
{
  QTE_D_CONST(qtActionManager);
  foreach (auto const& iter, qtEnumerate(d->staticActions))
    this->loadShortcut(iter.key(), settings, iter.value());
}

//-----------------------------------------------------------------------------
void qtActionManager::loadShortcut(QAction* action, QSettings& settings,
                                   QString settingsKey)
{
  // Get default shortcut(s)
  QVariantList values;
  foreach (auto const& seq, action->shortcuts())
    values.append(seq);

  // Load shortcut(s)
  QList<QKeySequence> shortcuts;
  values = settings.value("Shortcuts/" + settingsKey, values).toList();
  foreach (auto const& value, values)
    {
    if (value.canConvert<QKeySequence>())
      {
      shortcuts.append(value.value<QKeySequence>());
      }
    }
  action->setShortcuts(shortcuts);
}
