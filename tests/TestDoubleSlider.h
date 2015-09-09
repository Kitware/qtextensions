/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
  Q_DISABLE_COPY(TestDoubleSliderWidget)
};

#endif
