/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
