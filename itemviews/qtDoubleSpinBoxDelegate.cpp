// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtDoubleSpinBoxDelegate.h"

#include <QDoubleSpinBox>

#include "../core/qtMath.h"

QTE_IMPLEMENT_D_FUNC(qtDoubleSpinBoxDelegate)

//-----------------------------------------------------------------------------
class qtDoubleSpinBoxDelegatePrivate
{
public:
  qtDoubleSpinBoxDelegatePrivate();

  int precision;
  double minimum, maximum;
  Qt::ItemDataRole dataRole;
};

//-----------------------------------------------------------------------------
qtDoubleSpinBoxDelegatePrivate::qtDoubleSpinBoxDelegatePrivate()
  : precision(-1), minimum(qQNaN()), maximum(qQNaN()),
    dataRole(Qt::EditRole)
{
}

//-----------------------------------------------------------------------------
qtDoubleSpinBoxDelegate::qtDoubleSpinBoxDelegate(QObject* parent)
  : QStyledItemDelegate(parent), d_ptr(new qtDoubleSpinBoxDelegatePrivate)
{
}

//-----------------------------------------------------------------------------
qtDoubleSpinBoxDelegate::~qtDoubleSpinBoxDelegate()
{
}

//-----------------------------------------------------------------------------
void qtDoubleSpinBoxDelegate::setRange(double minimum, double maximum)
{
  QTE_D(qtDoubleSpinBoxDelegate);
  d->minimum = minimum;
  d->maximum = maximum;
}

//-----------------------------------------------------------------------------
void qtDoubleSpinBoxDelegate::setPrecision(int precision)
{
  QTE_D(qtDoubleSpinBoxDelegate);
  d->precision = precision;
}

//-----------------------------------------------------------------------------
void qtDoubleSpinBoxDelegate::setDataRole(Qt::ItemDataRole role)
{
  QTE_D(qtDoubleSpinBoxDelegate);
  d->dataRole = role;
}

//-----------------------------------------------------------------------------
QWidget* qtDoubleSpinBoxDelegate::createEditor(
  QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
  QTE_D_CONST(qtDoubleSpinBoxDelegate);

  QDoubleSpinBox* editor = new QDoubleSpinBox(parent);

  if (qIsFinite(d->minimum))
    {
    editor->setMinimum(d->minimum);
    }
  if (qIsFinite(d->maximum))
    {
    editor->setMaximum(d->maximum);
    }

  if (d->precision >= 0)
    {
    editor->setDecimals(d->precision);
    editor->setSingleStep(1.0 / pow(1e1, d->precision));
    }

  return editor;
}

//-----------------------------------------------------------------------------
void qtDoubleSpinBoxDelegate::updateEditorGeometry(
  QWidget* editor, const QStyleOptionViewItem& opt, const QModelIndex&) const
{
  editor->setGeometry(opt.rect);
}

//-----------------------------------------------------------------------------
void qtDoubleSpinBoxDelegate::setEditorData(
  QWidget* editor, const QModelIndex& index) const
{
  QTE_D_CONST(qtDoubleSpinBoxDelegate);

  QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
  Q_ASSERT(spinBox);
  spinBox->setValue(index.model()->data(index, d->dataRole).toDouble());
}

//-----------------------------------------------------------------------------
void qtDoubleSpinBoxDelegate::setModelData(
  QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QTE_D_CONST(qtDoubleSpinBoxDelegate);

  QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);
  Q_ASSERT(spinBox);

  spinBox->interpretText();
  if (d->dataRole != Qt::EditRole)
    {
    model->setData(index, spinBox->value(), d->dataRole);
    model->setData(index, spinBox->text(), Qt::EditRole);
    }
  else
    {
    model->setData(index, spinBox->value(), Qt::EditRole);
    }
}
