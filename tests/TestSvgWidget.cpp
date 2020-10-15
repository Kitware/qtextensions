// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "../widgets/qtSvgWidget.h"

#include <QApplication>

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    qtSvgWidget w;

    if (argc > 1)
    {
        w.setResource(argv[1]);
        w.resize(w.sizeHint());
    }

    w.show();
    return app.exec();
}
