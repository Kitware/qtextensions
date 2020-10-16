// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtDoubleInputDialog.h"

#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>

#include "../core/qtUtil.h"

QTE_IMPLEMENT_D_FUNC(qtDoubleInputDialog)

//-----------------------------------------------------------------------------
class qtDoubleInputDialogPrivate
{
public:
  QLabel* label_;
  QDoubleSpinBox* spinBox_;
  QDialogButtonBox* buttons_;
  QGridLayout* layout_;
};

//-----------------------------------------------------------------------------
qtDoubleInputDialog::qtDoubleInputDialog(QWidget* parent,
                                         Qt::WindowFlags flags)
  : QDialog(parent, flags), d_ptr(new qtDoubleInputDialogPrivate)
{
  QTE_D(qtDoubleInputDialog);
  d->layout_ = new QGridLayout;

  // Create controls (properties are set by getDouble)
  d->label_ = new QLabel;
  d->spinBox_ = new QDoubleSpinBox;
  d->buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
                                     QDialogButtonBox::Cancel);
  qtUtil::setStandardIcons(d->buttons_);

  // Connect buttons
  connect(d->buttons_, SIGNAL(accepted()), this, SLOT(accept()));
  connect(d->buttons_, SIGNAL(rejected()), this, SLOT(reject()));

  // Layout controls
  d->layout_->addWidget(d->label_, 0, 0);
  d->layout_->addWidget(d->spinBox_, 0, 1);
  d->layout_->addWidget(d->buttons_, 1, 0, 1, -1);
  this->setLayout(d->layout_);
}

//-----------------------------------------------------------------------------
qtDoubleInputDialog::~qtDoubleInputDialog()
{
  delete this->d_ptr;
}

//-----------------------------------------------------------------------------
void qtDoubleInputDialog::setLabel(const QString& label)
{
  QTE_D(qtDoubleInputDialog);
  d->label_->setText(label);
}

//-----------------------------------------------------------------------------
void qtDoubleInputDialog::setSpinProperties(
  double value, double minValue, double maxValue, int decimals, double step)
{
  QTE_D(qtDoubleInputDialog);
  d->spinBox_->setRange(minValue, maxValue);
  d->spinBox_->setDecimals(decimals);
  d->spinBox_->setSingleStep(step);
  d->spinBox_->setValue(value);
}

//-----------------------------------------------------------------------------
void qtDoubleInputDialog::setSpecialValueText(const QString& special)
{
  QTE_D(qtDoubleInputDialog);
  d->spinBox_->setSpecialValueText(special);
}

//-----------------------------------------------------------------------------
double qtDoubleInputDialog::value()
{
  QTE_D(qtDoubleInputDialog);
  return d->spinBox_->value();
}

//-----------------------------------------------------------------------------
double qtDoubleInputDialog::getDouble(
  QWidget* parent, const QString& title, const QString& label, double value,
  double minValue, double maxValue, bool* okay, Qt::WindowFlags flags)
{
  return qtDoubleInputDialog::getDouble(
    parent, title, label, value, minValue, maxValue,
    QString(), 1, 1.0, okay, flags);
}

//-----------------------------------------------------------------------------
double qtDoubleInputDialog::getDouble(
  QWidget* parent, const QString& title, const QString& label, double value,
  double minValue, double maxValue, int decimals, double step, bool* okay,
  Qt::WindowFlags flags)
{
  return qtDoubleInputDialog::getDouble(
    parent, title, label, value, minValue, maxValue,
    QString(), decimals, step, okay, flags);
}

//-----------------------------------------------------------------------------
double qtDoubleInputDialog::getDouble(
  QWidget* parent, const QString& title, const QString& label, double value,
  double minValue, double maxValue, const QString& specialValueText,
  int decimals, double step, bool* okay, Qt::WindowFlags flags)
{
  // Set up dialog
  qtDoubleInputDialog* self = new qtDoubleInputDialog(parent, flags);
  self->setWindowTitle(title);
  self->setLabel(label);
  self->setSpinProperties(value, minValue, maxValue, decimals, step);
  self->setSpecialValueText(specialValueText);

  // Execute dialog
  int result = self->exec();
  if (okay)
    {
    *okay = (result == QDialog::Accepted);
    }
  if (result == QDialog::Accepted)
    {
    value = self->value();
    }

  // Done
  delete self;
  return value;
}
