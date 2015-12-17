/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QApplication>

#include "../dialogs/qtGradientEditor.h"

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app{argc, argv};

    qtGradientEditor editor;

    return editor.exec();
}
