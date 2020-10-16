// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __TestDoubleSlider_h
#define __TestDoubleSlider_h

#include "ui_testDoubleSlider.h"

class TestDoubleSliderWidget : public QWidget
{
  Q_OBJECT

public:
  explicit TestDoubleSliderWidget(QWidget* parent = 0);
  ~TestDoubleSliderWidget() {}

protected slots:
  void setQuantizeMode(int);
  void setValue(double);

protected:
  Ui::TestDoubleSliderWidget UI;

private:
  QTE_DISABLE_COPY(TestDoubleSliderWidget)
};

#endif
