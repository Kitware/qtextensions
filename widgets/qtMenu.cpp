// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtMenu.h"

#include <QHelpEvent>
#include <QToolTip>

#include <QDebug>

namespace // anonymous
{

//-----------------------------------------------------------------------------
bool hasToolTip(QAction* action)
{
    auto const& text = action->toolTip();

    auto simplifiedText = action->text();
    simplifiedText.replace(QRegExp("[&](?=[^&])"), "");

    return !text.isEmpty() && text != simplifiedText;
}

}

//-----------------------------------------------------------------------------
qtMenu::qtMenu(QWidget* parent) : QMenu(parent)
{
}

//-----------------------------------------------------------------------------
qtMenu::~qtMenu()
{
}

//-----------------------------------------------------------------------------
bool qtMenu::event(QEvent* e)
{
    if (e->type() == QEvent::ToolTip)
    {
        auto const helpEvent = static_cast <QHelpEvent *>(e);
        auto const action = this->actionAt(helpEvent->pos());

        if (action && hasToolTip(action))
            QToolTip::showText(helpEvent->globalPos(), action->toolTip());
        else
            QToolTip::hideText();

        return true;
    }

    return QMenu::event(e);
}
