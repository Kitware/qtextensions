/*ckwg +5
 * Copyright 2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
