// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
