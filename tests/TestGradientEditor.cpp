/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
