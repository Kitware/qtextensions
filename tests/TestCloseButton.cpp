// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
