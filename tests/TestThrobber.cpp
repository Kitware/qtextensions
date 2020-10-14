// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
