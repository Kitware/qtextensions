/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
