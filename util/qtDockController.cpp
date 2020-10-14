// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QAction>
#include <QDebug>
#include <QDockWidget>

#include "qtDockControllerPrivate.h"

QTE_IMPLEMENT_D_FUNC(qtDockController)

//-----------------------------------------------------------------------------
qtDockControllerPrivate::qtDockControllerPrivate()
{
  mapper = new QSignalMapper(this);

  connect(mapper, SIGNAL(mapped(QObject*)),
          this, SLOT(toggled(QObject*)));
}

//-----------------------------------------------------------------------------
void qtDockControllerPrivate::toggled(QObject* obj)
{
  QAction* internal = qobject_cast<QAction*>(obj);
  QAction* external = this->map[obj];
  bool restore = external->blockSignals(true);
  external->setChecked(internal->isChecked());
  external->blockSignals(restore);
}

//-----------------------------------------------------------------------------
qtDockController::qtDockController(QObject* parent)
  : QObject(parent), d_ptr(new qtDockControllerPrivate)
{
}

//-----------------------------------------------------------------------------
qtDockController::~qtDockController()
{
}

//-----------------------------------------------------------------------------
void qtDockController::addToggleAction(QAction* external, QDockWidget* dock)
{
  QTE_D(qtDockController);

  QAction* internal = dock->toggleViewAction();

  d->mapper->setMapping(internal, internal);
  d->map.insert(internal, external);

  connect(external, SIGNAL(toggled(bool)),
          internal, SIGNAL(triggered(bool)));
  connect(internal, SIGNAL(toggled(bool)),
          d->mapper, SLOT(map()));

  external->setChecked(internal->isChecked());
}
