// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QApplication>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>

#include "../widgets/qtColorButton.h"

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QWidget window;
  QFormLayout* layout = new QFormLayout;
  window.setMinimumSize(160, 0);
  window.setLayout(layout);

  qtColorButton* button = new qtColorButton;
  QCheckBox* alpha = new QCheckBox;

  window.connect(alpha, SIGNAL(toggled(bool)),
                 button, SLOT(setAlphaShown(bool)));

  layout->addRow(new QLabel("Color:"), button);
  layout->addRow(new QLabel("Alpha:"), alpha);

  window.show();
  return app.exec();
}
