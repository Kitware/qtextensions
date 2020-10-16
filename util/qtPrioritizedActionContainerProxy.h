// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtPrioritizedActionContainerProxy_h
#define __qtPrioritizedActionContainerProxy_h

#include <QAction>

#include <limits>

#include "qtActionPriorityList.h"
#include "qtUtilNamespace.h"

//-----------------------------------------------------------------------------
template <typename ContainerType>
class qtPrioritizedActionContainerProxy : protected qtActionPriorityList
{
public:
  qtPrioritizedActionContainerProxy(
    ContainerType* container, QAction* followingAction,
    qtUtil::SeparatorBehavior separators);

  void insertAction(QAction* action, int priority);
  QAction* insertSeparator(int priority);

  template <typename T>
  QAction* insertObject(T* object, int priority,
                        QAction* (ContainerType::*method)(QAction*, T*));

  ContainerType* const Container;

protected:
  void addInitialSeparator();

  qtUtil::SeparatorBehavior Separators;
};

//-----------------------------------------------------------------------------
template <typename ContainerType>
qtPrioritizedActionContainerProxy<ContainerType>::
qtPrioritizedActionContainerProxy(
  ContainerType* container, QAction* followingAction,
  qtUtil::SeparatorBehavior separators) :
  qtActionPriorityList(followingAction),
  Container(container),
  Separators(separators)
{
}

//-----------------------------------------------------------------------------
template <typename ContainerType>
void qtPrioritizedActionContainerProxy<ContainerType>::addInitialSeparator()
{
  if (this->Separators)
    {
    const qtUtil::SeparatorBehavior separators = this->Separators;
    this->Separators = qtUtil::NoSeparators; // Prevent recursion

    if (separators & qtUtil::AddSeparatorBefore)
      {
      const int firstPriority = std::numeric_limits<int>::min();
      QAction* const first = this->followingAction(firstPriority);
      this->Container->insertSeparator(first);
      }

    if (separators & qtUtil::AddSeparatorAfter)
      {
      const int lastPriority = std::numeric_limits<int>::max();
      QAction* const last = this->followingAction(lastPriority);
      QAction* const separator = this->Container->insertSeparator(last);
      this->insertAction(separator, lastPriority);
      }
    }
}

//-----------------------------------------------------------------------------
template <typename ContainerType>
void qtPrioritizedActionContainerProxy<ContainerType>::insertAction(
  QAction* action, int priority)
{
  // Add separators, if not already present
  this->addInitialSeparator();

  // Get action to follow the one being inserted
  QAction* const before = this->followingAction(priority);

  // Insert new action and update map of inserted actions
  this->Container->insertAction(before, action);
  this->qtActionPriorityList::insertAction(action, priority);
}

//-----------------------------------------------------------------------------
template <typename ContainerType>
QAction* qtPrioritizedActionContainerProxy<ContainerType>::insertSeparator(
  int priority)
{
  // Add separators, if not already present
  this->addInitialSeparator();

  // Get action to follow the one being inserted
  QAction* const before = this->followingAction(priority);

  // Insert new action and update map of inserted actions
  QAction* const action = this->Container->insertSeparator(before);
  this->insertAction(action, priority);

  // Return action
  return action;
}

//-----------------------------------------------------------------------------
template <typename ContainerType>
template <typename T>
QAction* qtPrioritizedActionContainerProxy<ContainerType>::insertObject(
  T* object, int priority, QAction* (ContainerType::*method)(QAction*, T*))
{
  // Add separators, if not already present
  this->addInitialSeparator();

  // Get action to follow the one being inserted
  QAction* const before = this->followingAction(priority);

  // Insert new action and update map of inserted actions
  QAction* const action = (this->Container->*method)(before, object);
  this->insertAction(action, priority);

  // Return action
  return action;
}

#endif
