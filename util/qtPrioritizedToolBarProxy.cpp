// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtPrioritizedToolBarProxy.h"

#include <QToolBar>

#include "qtPrioritizedActionContainerProxy.h"

QTE_IMPLEMENT_D_FUNC(qtPrioritizedToolBarProxy)

//-----------------------------------------------------------------------------
class qtPrioritizedToolBarProxyPrivate :
  public qtPrioritizedActionContainerProxy<QToolBar>
{
public:
  qtPrioritizedToolBarProxyPrivate(QToolBar* toolBar, QAction* followingAction,
                                   qtUtil::SeparatorBehavior seps) :
    qtPrioritizedActionContainerProxy<QToolBar>(toolBar, followingAction, seps)
    {}
};

//-----------------------------------------------------------------------------
qtPrioritizedToolBarProxy::qtPrioritizedToolBarProxy(
  QToolBar* toolBar, QAction* followingAction, qtUtil::SeparatorBehavior seps) :
  d_ptr(new qtPrioritizedToolBarProxyPrivate(toolBar, followingAction, seps))
{
}

//-----------------------------------------------------------------------------
qtPrioritizedToolBarProxy::~qtPrioritizedToolBarProxy()
{
}

//-----------------------------------------------------------------------------
const QToolBar* qtPrioritizedToolBarProxy::toolBar() const
{
  QTE_D_CONST(qtPrioritizedToolBarProxy);
  return d->Container;
}

//-----------------------------------------------------------------------------
void qtPrioritizedToolBarProxy::insertAction(QAction* action, int priority)
{
  QTE_D(qtPrioritizedToolBarProxy);
  d->insertAction(action, priority);
}

//-----------------------------------------------------------------------------
QAction* qtPrioritizedToolBarProxy::insertSeparator(int priority)
{
  QTE_D(qtPrioritizedToolBarProxy);
  return d->insertSeparator(priority);
}

//-----------------------------------------------------------------------------
QAction* qtPrioritizedToolBarProxy::insertWidget(QWidget* widget, int priority)
{
  QTE_D(qtPrioritizedToolBarProxy);
  return d->insertObject(widget, priority, &QToolBar::insertWidget);
}
