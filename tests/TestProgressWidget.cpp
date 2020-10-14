// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
