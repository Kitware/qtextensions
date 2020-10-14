// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtColorButtonItemWidget_h
#define __qtColorButtonItemWidget_h

#include "../widgets/qtColorButton.h"

class QTreeWidgetItem;

class qtColorButtonItemWidgetPrivate;

/// Item view widget based on qtColorButton.
///
/// qtColorButtonItemWidget is a subclass of qtColorButton that is designed to
/// be used as the item widget for an index in an item view. In addition to the
/// features of qtColorButton, it automatically sets its initial color to the
/// data of its corresponding item index, and updates the same when the user
/// selects a different color.
///
/// The item color should be stored in Qt::EditRole.
class QTE_EXPORT qtColorButtonItemWidget : public qtColorButton
{
  Q_OBJECT

public:
  /// Create item widget for QTreeWidgetItem.
  ///
  /// This constructs a color button for use by a QTreeWidgetItem. If the item
  /// has been added to the tree (item->treeWidget() is not null), the widget
  /// will be parented to the QTreeWidget and will set itself as the item
  /// widget for the specified item and column.
  ///
  /// \param item QTreeWidgetItem for which the item widget will be used.
  /// \param column Data column for which the item widget will be used.
  qtColorButtonItemWidget(QTreeWidgetItem* item, int column);
  virtual ~qtColorButtonItemWidget();

  /// \copydoc qtColorButton::setColor
  ///
  /// This also sets the color in the corresponding item's data.
  virtual void setColor(QColor) QTE_OVERRIDE;

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtColorButtonItemWidget)

private:
  QTE_DECLARE_PRIVATE(qtColorButtonItemWidget)
  QTE_DISABLE_COPY(qtColorButtonItemWidget)
};

#endif
