/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "TestExpander.h"

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "../widgets/qtExpander.h"

//-----------------------------------------------------------------------------
TestExpanderWidget::TestExpanderWidget(QWidget* parent) : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout;

  this->label = new QLabel("Hello, world!", this);
  this->expander = new qtExpander(true, this);
  this->lineEdit = new QLineEdit(this);

  layout->addWidget(this->lineEdit);
  layout->addWidget(this->expander);
  layout->addWidget(this->label);

  this->setLayout(layout);

  connect(this->lineEdit, SIGNAL(textChanged(QString)),
          this, SLOT(setExpanderText(QString)));
  connect(this->expander, SIGNAL(toggled(bool)),
          this->label, SLOT(setVisible(bool)));
}

//-----------------------------------------------------------------------------
void TestExpanderWidget::setExpanderText(QString newText)
{
  this->expander->setText(newText);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  TestExpanderWidget window;
  window.show();
  return app.exec();
}
