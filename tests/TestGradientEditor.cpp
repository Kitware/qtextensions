// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QApplication>
#include <QDebug>

#include "../dialogs/qtGradientEditor.h"

#include "../util/qtGradient.h"

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    qtGradientEditor editor;

    auto const result = editor.exec();
    if (result == QDialog::Accepted)
    {
        foreach (auto const stop, editor.gradient().stops())
        {
            qDebug().nospace()
                << "gradient.insertStop("
                << stop.position << ", QColor("
                << stop.color.red() << ", "
                << stop.color.green() << ", "
                << stop.color.blue() << "));";
        }
        return 0;
    }

    // Dialog canceled
    return 1;
}
