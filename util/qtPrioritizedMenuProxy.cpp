// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtPrioritizedMenuProxy.h"

#include <QMenu>

#include "qtPrioritizedActionContainerProxy.h"

QTE_IMPLEMENT_D_FUNC(qtPrioritizedMenuProxy)

//-----------------------------------------------------------------------------
class qtPrioritizedMenuProxyPrivate :
  public qtPrioritizedActionContainerProxy<QMenu>
{
public:
  qtPrioritizedMenuProxyPrivate(QMenu* menu, QAction* followingAction,
                                qtUtil::SeparatorBehavior seps) :
    qtPrioritizedActionContainerProxy<QMenu>(menu, followingAction, seps)
    {}
};

//-----------------------------------------------------------------------------
qtPrioritizedMenuProxy::qtPrioritizedMenuProxy(
  QMenu* menu, QAction* followingAction, qtUtil::SeparatorBehavior seps) :
  d_ptr(new qtPrioritizedMenuProxyPrivate(menu, followingAction, seps))
{
}

//-----------------------------------------------------------------------------
qtPrioritizedMenuProxy::~qtPrioritizedMenuProxy()
{
}

//-----------------------------------------------------------------------------
const QMenu* qtPrioritizedMenuProxy::menu() const
{
  QTE_D_CONST(qtPrioritizedMenuProxy);
  return d->Container;
}

//-----------------------------------------------------------------------------
void qtPrioritizedMenuProxy::insertAction(QAction* action, int priority)
{
  QTE_D(qtPrioritizedMenuProxy);
  d->insertAction(action, priority);
}

//-----------------------------------------------------------------------------
QAction* qtPrioritizedMenuProxy::insertSeparator(int priority)
{
  QTE_D(qtPrioritizedMenuProxy);
  return d->insertSeparator(priority);
}

//-----------------------------------------------------------------------------
QAction* qtPrioritizedMenuProxy::insertMenu(QMenu* menu, int priority)
{
  QTE_D(qtPrioritizedMenuProxy);
  return d->insertObject(menu, priority, &QMenu::insertMenu);
}
