/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QTreeWidget>

#include "qtScopedValueChange.h"

//-----------------------------------------------------------------------------
bool qtSvcWidgetUpdateHelper::get(QWidget* widget)
{
  return widget->updatesEnabled();
}

//-----------------------------------------------------------------------------
void qtSvcWidgetUpdateHelper::set(QWidget* widget, bool value)
{
  return widget->setUpdatesEnabled(value);
}

//-----------------------------------------------------------------------------
bool qtSvcTreeSortHelper::get(QTreeWidget* tree)
{
  return tree->isSortingEnabled();
}

//-----------------------------------------------------------------------------
void qtSvcTreeSortHelper::set(QTreeWidget* tree, bool value)
{
  tree->setSortingEnabled(value);
}
