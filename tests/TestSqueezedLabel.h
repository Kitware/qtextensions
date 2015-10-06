/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __TestSqueezedLabel_h
#define __TestSqueezedLabel_h

#include "../core/qtGlobal.h"

#include <QWidget>

class QCheckBox;
class QLabel;
class QTextEdit;

class qtSqueezedLabel;

class TestSqueezedLabelWidget : public QWidget
{
  Q_OBJECT

public:
  TestSqueezedLabelWidget(QWidget* parent = 0);
  virtual ~TestSqueezedLabelWidget() {}

public slots:
  void setElideMode();
  void setLabelText();

protected:
  qtSqueezedLabel* label;
  QTextEdit* textEdit;
  QCheckBox* elideStart, *elideEnd, *elideFade;

private:
  QTE_DISABLE_COPY(TestSqueezedLabelWidget)
};

#endif
