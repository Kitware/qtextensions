// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtNumericTreeWidgetItem_h
#define __qtNumericTreeWidgetItem_h

#include <QTreeWidgetItem>

class qtNumericTreeWidgetItem : public QTreeWidgetItem
{
public:
  /// Sets the value to be displayed in \p column to \p value.
  ///
  /// Tree items whose values have been set to the same data type will be
  /// sorted numerically. \p value must be of a type that can be converted to a
  /// QVariant.
  template <typename T> void setValue(int column, T value)
    { this->setData(column, Qt::DisplayRole, QVariant::fromValue<T>(value)); }
};

#endif
