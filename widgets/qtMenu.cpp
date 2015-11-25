/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
