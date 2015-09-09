/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtSpinBoxDelegate.h"

#include <QSpinBox>

#include "../core/qtMath.h"

QTE_IMPLEMENT_D_FUNC(qtSpinBoxDelegate)

//-----------------------------------------------------------------------------
class qtSpinBoxDelegatePrivate
{
public:
  qtSpinBoxDelegatePrivate();

  int Minimum, Maximum;
  Qt::ItemDataRole DataRole;
  Qt::Alignment Alignment;
};

//-----------------------------------------------------------------------------
qtSpinBoxDelegatePrivate::qtSpinBoxDelegatePrivate() :
  Minimum(0), Maximum(99), DataRole(Qt::EditRole),
  Alignment(Qt::AlignLeft | Qt::AlignVCenter)
{
}

//-----------------------------------------------------------------------------
qtSpinBoxDelegate::qtSpinBoxDelegate(QObject* parent)
  : QStyledItemDelegate(parent), d_ptr(new qtSpinBoxDelegatePrivate)
{
}

//-----------------------------------------------------------------------------
qtSpinBoxDelegate::~qtSpinBoxDelegate()
{
}

//-----------------------------------------------------------------------------
void qtSpinBoxDelegate::setDataRole(Qt::ItemDataRole role)
{
  QTE_D(qtSpinBoxDelegate);
  d->DataRole = role;
}

//-----------------------------------------------------------------------------
void qtSpinBoxDelegate::setRange(int minimum, int maximum)
{
  QTE_D(qtSpinBoxDelegate);
  d->Minimum = minimum;
  d->Maximum = maximum;
}

//-----------------------------------------------------------------------------
void qtSpinBoxDelegate::setAlignment(Qt::Alignment alignment)
{
  QTE_D(qtSpinBoxDelegate);
  d->Alignment = alignment;
}

//-----------------------------------------------------------------------------
QWidget* qtSpinBoxDelegate::createEditor(
  QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
  QTE_D_CONST(qtSpinBoxDelegate);

  QSpinBox* editor = new QSpinBox(parent);
  editor->setRange(d->Minimum, d->Maximum);
  editor->setAlignment(d->Alignment);

  return editor;
}

//-----------------------------------------------------------------------------
void qtSpinBoxDelegate::updateEditorGeometry(
  QWidget* editor, const QStyleOptionViewItem& opt, const QModelIndex&) const
{
  editor->setGeometry(opt.rect);
}

//-----------------------------------------------------------------------------
void qtSpinBoxDelegate::setEditorData(
  QWidget* editor, const QModelIndex& index) const
{
  QTE_D_CONST(qtSpinBoxDelegate);

  QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
  Q_ASSERT(spinBox);
  spinBox->setValue(index.model()->data(index, d->DataRole).toInt());
}

//-----------------------------------------------------------------------------
void qtSpinBoxDelegate::setModelData(
  QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QTE_D_CONST(qtSpinBoxDelegate);

  QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
  Q_ASSERT(spinBox);

  spinBox->interpretText();
  if (d->DataRole != Qt::EditRole)
    {
    model->setData(index, spinBox->value(), d->DataRole);
    model->setData(index, spinBox->text(), Qt::EditRole);
    }
  else
    {
    model->setData(index, spinBox->value(), Qt::EditRole);
    }
}
