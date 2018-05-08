/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QApplication>

#include "../widgets/qtProgressWidget.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  qtProgressWidget widget;
  widget.setNameVisible(true);
  widget.addProgressBar("Task 1", "Description 1", 20);
  widget.setValue("Task 2", 20);
  widget.setRange("Task 2", 0, 40);
  widget.setDescription("Task 3", "Description 3");
  widget.addProgressBar("Task 4", "Description 4", 0, 0, 0);

  widget.show();
  return app.exec();
}
