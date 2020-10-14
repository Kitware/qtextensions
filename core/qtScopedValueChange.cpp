// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
