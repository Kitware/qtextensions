/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "../widgets/qtProgressWidget.h"

#include <QApplication>

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    QApplication app{argc, argv};

    qtProgressWidget widget;
    widget.setLabelAlignment(Qt::AlignCenter);

    auto const task1 = widget.addTask("This is a sample task", 20);
    auto const task2 = widget.addTask("This task is 'busy'");
    widget.setProgressRange(task2, 0, 0);

    widget.show();
    return app.exec();
}
