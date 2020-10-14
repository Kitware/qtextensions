// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
