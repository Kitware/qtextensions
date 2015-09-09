/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __TestExpander_h
#define __TestExpander_h

#include <QWidget>

class QLabel;
class QLineEdit;

class qtExpander;

class TestExpanderWidget : public QWidget
{
  Q_OBJECT

public:
  TestExpanderWidget(QWidget* parent = 0);
  virtual ~TestExpanderWidget() {}

public slots:
  void setExpanderText(QString);

protected:
  QLineEdit* lineEdit;
  qtExpander* expander;
  QLabel* label;

private:
  Q_DISABLE_COPY(TestExpanderWidget)
};

#endif
