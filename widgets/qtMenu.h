// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
    qtMenu(QWidget* parent = nullptr);
    virtual ~qtMenu();

protected:
    virtual bool event(QEvent*);

private:
    QTE_DISABLE_COPY(qtMenu)
};

#endif
