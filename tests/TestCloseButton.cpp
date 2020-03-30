/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QApplication>
#include <QLabel>
#include <QTextEdit>
#include <QGridLayout>

#include "../widgets/qtCloseButton.h"

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget window;

    auto* const layout = new QGridLayout;

    auto* const label = new QLabel{"This is a title", &window};
    auto* const button = new qtCloseButton{&window};
    auto* const editor = new QTextEdit{&window};

    layout->addWidget(label, 0, 0);
    layout->addWidget(button, 0, 1);
    layout->addWidget(editor, 1, 0, 1, 2);

    window.setLayout(layout);

    window.show();
    editor->setFocus();

    return app.exec();
}
