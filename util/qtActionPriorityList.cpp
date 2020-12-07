// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  return (iter == d->end() ? nullptr : iter.value());
}

//-----------------------------------------------------------------------------
QAction* qtActionPriorityList::followingAction(int priority) const
{
  QTE_D_SHARED(qtActionPriorityList);

  const qtActionPriorityListData::const_iterator iter = d->lowerBound(priority);
  return (iter == d->constEnd() ? nullptr : iter.value());
}
