// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __TestExpander_h
#define __TestExpander_h

#include "../core/qtGlobal.h"

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
  QTE_DISABLE_COPY(TestExpanderWidget)
};

#endif
