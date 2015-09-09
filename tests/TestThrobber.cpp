/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QApplication>
#include <QLabel>

#include "../widgets/qtOverlayWidget.h"
#include "../widgets/qtThrobber.h"

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  qtOverlayWidget overlay;

  qtThrobber* throbber = new qtThrobber(&overlay);
  throbber->setActive(true);
  throbber->setMaxThrobberSize(200);

  QLabel* label = new QLabel(&overlay);
  label->setText("Throbber and Overlay Test");
  label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

  overlay.addWidget(label);
  overlay.addWidget(throbber);

  overlay.show();
  return app.exec();
}
