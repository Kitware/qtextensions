/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtActionPriorityList_h
#define __qtActionPriorityList_h

#include <QSharedDataPointer>

#include "../core/qtGlobal.h"

class QAction;

class qtActionPriorityListData;

/// Action list with priority.
///
/// This class implements a simple action list with priority-based insertion.
/// Inserted actions are ordered according to an integer priority value.
/// New actions are inserted after existing actions of lower priority, and
/// before actions of lesser or equal priority.
///
/// This class is used to implement qtPrioritizedMenuProxy and
/// qtPrioritizedToolBarProxy.
class QTE_EXPORT qtActionPriorityList
{
public:
  /// Create action list.
  ///
  /// \param followingAction Pointer to an action which will follow (come
  ///                        after) actions inserted into this list.
  qtActionPriorityList(QAction* followingAction = 0);

  /// Copy constructor.
  qtActionPriorityList(const qtActionPriorityList&);

  ~qtActionPriorityList();

  /// Assignment operator.
  qtActionPriorityList& operator=(const qtActionPriorityList&);

  /// Insert a new action into the list.
  ///
  /// \return Pointer to the action immediately following the newly inserted
  ///         action, or 0 if the action was inserted at the end of the list.
  QAction* insertAction(QAction*, int priority = 0);

  /// Obtain action following specified priority.
  ///
  /// \return Pointer to the action that would immediately follow a newly
  ///         inserted action of \p priority, or 0 if the action would be
  ///         inserted at the end of the list.
  QAction* followingAction(int priority) const;

protected:
  QTE_DECLARE_SHARED_PTR(qtActionPriorityList)

private:
  QTE_DECLARE_SHARED(qtActionPriorityList)
};

#endif
