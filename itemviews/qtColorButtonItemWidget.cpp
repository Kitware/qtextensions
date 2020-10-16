// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtColorButtonItemWidget.h"

#include <QTreeWidgetItem>

QTE_IMPLEMENT_D_FUNC(qtColorButtonItemWidget)

//-----------------------------------------------------------------------------
class qtColorButtonItemWidgetPrivate
{
public:
  qtColorButtonItemWidgetPrivate(QTreeWidgetItem* item, int column) :
    Item(item), Column(column) {}

  QTreeWidgetItem* const Item;
  const int Column;

  QColor color();
  void setColor(QColor);
};

//-----------------------------------------------------------------------------
QColor qtColorButtonItemWidgetPrivate::color()
{
  if (this->Item)
    {
    const QVariant data = this->Item->data(this->Column, Qt::EditRole);
    if (data.isValid() && data.canConvert<QColor>())
      {
      return data.value<QColor>();
      }
    }
  return {};
}

//-----------------------------------------------------------------------------
void qtColorButtonItemWidgetPrivate::setColor(QColor c)
{
  if (this->Item)
    {
    this->Item->setData(this->Column, Qt::EditRole, c);
    }
}

//-----------------------------------------------------------------------------
qtColorButtonItemWidget::qtColorButtonItemWidget(
  QTreeWidgetItem* item, int column) :
  qtColorButton(item->treeWidget()),
  d_ptr(new qtColorButtonItemWidgetPrivate(item, column))
{
  QTE_D(qtColorButtonItemWidget);

  this->setColor(d->color());
  this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

  if (item->treeWidget())
    {
    item->treeWidget()->setItemWidget(item, column, this);
    }
}

//-----------------------------------------------------------------------------
qtColorButtonItemWidget::~qtColorButtonItemWidget()
{
}

//-----------------------------------------------------------------------------
void qtColorButtonItemWidget::setColor(QColor c)
{
  QTE_D(qtColorButtonItemWidget);

  if (c != d->color())
    {
    d->setColor(c);
    }

  qtColorButton::setColor(c);
}
