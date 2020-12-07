// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "TestSqueezedLabel.h"

#include <QApplication>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QTextEdit>

#include "../widgets/qtSqueezedLabel.h"

static const char* defaultText =
  "Hello, world!\n\nThis is an example of long text that does not fit.";

//-----------------------------------------------------------------------------
TestSqueezedLabelWidget::TestSqueezedLabelWidget(QWidget* parent)
  : QWidget(parent)
{
  QFormLayout* layout = new QFormLayout;
  this->setLayout(layout);

  this->label = new qtSqueezedLabel;
  this->elideStart = new QCheckBox("Elide Start");
  this->elideEnd = new QCheckBox("Elide End");
  this->elideFade = new QCheckBox("Elide Fade");
  this->textEdit = new QTextEdit;

  this->elideStart->setEnabled(false);
  this->elideEnd->setEnabled(false);
  this->elideEnd->setChecked(true);

  connect(this->elideStart, SIGNAL(toggled(bool)),
          this, SLOT(setElideMode()));
  connect(this->elideEnd, SIGNAL(toggled(bool)),
          this, SLOT(setElideMode()));
  connect(this->elideFade, SIGNAL(toggled(bool)),
          this, SLOT(setElideMode()));
  connect(this->textEdit, SIGNAL(textChanged()),
          this, SLOT(setLabelText()));

  layout->addRow("Label:", label);
  layout->addRow(this->elideStart);
  layout->addRow(this->elideEnd);
  layout->addRow(this->elideFade);
  layout->addRow("Text:", this->textEdit);

  this->resize(320, 240);

  this->textEdit->setText(defaultText);
  this->setElideMode();
}

//-----------------------------------------------------------------------------
void TestSqueezedLabelWidget::setElideMode()
{
  auto mode = qtSqueezedLabel::ElideMode{};
//   if (this->elideStart->isChecked())
//     mode |= qtSqueezedLabel::ElideStart;
  if (this->elideEnd->isChecked())
    mode |= qtSqueezedLabel::ElideEnd;
  if (this->elideFade->isChecked())
    mode |= qtSqueezedLabel::ElideFade;

  this->label->setElideMode(mode);
}

//-----------------------------------------------------------------------------
void TestSqueezedLabelWidget::setLabelText()
{
  this->label->setText(this->textEdit->toPlainText(),
                       qtSqueezedLabel::SetFullText |
                       qtSqueezedLabel::SetToolTip |
                       qtSqueezedLabel::SimplifyText);
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  TestSqueezedLabelWidget window;

  window.show();
  return app.exec();
}
