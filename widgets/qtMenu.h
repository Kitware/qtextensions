/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtMenu_h
#define __qtMenu_h

#include <QMenu>

#include "../core/qtGlobal.h"

/// Menu with tooltips.
///
/// This class enhances QMenu to also display tool tips for menu actions.
/// Tooltips are displayed only for actions that have non-trivial tooltips
/// (i.e. their tooltips are non-empty and differ from their normal text which
/// is already visible in the menu).
class QTE_EXPORT qtMenu : public QMenu
{
    Q_OBJECT

public:
    qtMenu(QWidget* parent = 0);
    virtual ~qtMenu();

protected:
    virtual bool event(QEvent*);

private:
    QTE_DISABLE_COPY(qtMenu)
};

#endif
