// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
