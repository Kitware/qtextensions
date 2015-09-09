/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "TestDoubleSlider.h"

#include "../core/qtScopedValueChange.h"

//-----------------------------------------------------------------------------
TestDoubleSliderWidget::TestDoubleSliderWidget(QWidget* parent)
  : QWidget(parent)
{
  this->UI.setupUi(this);

  connect(this->UI.slider, SIGNAL(valueChanged(double)),
          this, SLOT(setValue(double)));
  connect(this->UI.qmode, SIGNAL(currentIndexChanged(int)),
          this, SLOT(setQuantizeMode(int)));

  this->setValue(this->UI.slider->value());
}

//-----------------------------------------------------------------------------
void TestDoubleSliderWidget::setQuantizeMode(int mode)
{
  qtDoubleSlider::QuantizeMode qmode =
    (mode == 1 ? qtDoubleSlider::QuantizeAbsolute
               : qtDoubleSlider::QuantizeRelative);
  this->UI.slider->setQuantizeMode(qmode);
}

//-----------------------------------------------------------------------------
void TestDoubleSliderWidget::setValue(double value)
{
  qtScopedBlockSignals bs(this->UI.sval);
  this->UI.sval->setValue(value);
  this->UI.val->setText(QString::number(value));
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  TestDoubleSliderWidget window;
  window.show();
  return app.exec();
}
