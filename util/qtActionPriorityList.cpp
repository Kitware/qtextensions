/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtActionPriorityList.h"

#include <QMap>
#include <QMenu>

#include <limits>

QTE_IMPLEMENT_D_FUNC_SHARED(qtActionPriorityList)

//-----------------------------------------------------------------------------
class qtActionPriorityListData :
  public QSharedData, public QMultiMap<int, QAction*>
{
};

//-----------------------------------------------------------------------------
qtActionPriorityList::qtActionPriorityList(QAction* followingAction) :
  d_ptr(new qtActionPriorityListData)
{
  QTE_D_MUTABLE(qtActionPriorityList);
  d->insert(std::numeric_limits<int>::max(), followingAction);
}

//-----------------------------------------------------------------------------
qtActionPriorityList::qtActionPriorityList(const qtActionPriorityList& other) :
  d_ptr(other.d_ptr)
{
}

//-----------------------------------------------------------------------------
qtActionPriorityList::~qtActionPriorityList()
{
}

//-----------------------------------------------------------------------------
qtActionPriorityList& qtActionPriorityList::operator=(
  const qtActionPriorityList& other)
{
  this->d_ptr = other.d_ptr;
  return *this;
}

//-----------------------------------------------------------------------------
QAction* qtActionPriorityList::insertAction(QAction* action, int priority)
{
  QTE_D_MUTABLE(qtActionPriorityList);

  const qtActionPriorityListData::iterator iter = ++d->insert(priority, action);
  return (iter == d->end() ? 0 : iter.value());
}

//-----------------------------------------------------------------------------
QAction* qtActionPriorityList::followingAction(int priority) const
{
  QTE_D_SHARED(qtActionPriorityList);

  const qtActionPriorityListData::const_iterator iter = d->lowerBound(priority);
  return (iter == d->constEnd() ? 0 : iter.value());
}
