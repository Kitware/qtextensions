// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtThrobberInterface_h
#define __qtThrobberInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtThrobber.h"

class qtThrobberInterface
    : public qtDesignerWidgetInterfaceHelper<qtThrobber>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtThrobberInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtThrobber>(parent)
    {
    }

    virtual bool isContainer() const QTE_OVERRIDE
    {
        return true;
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A widget that draws a busy indicator";
    }

private:
    QTE_DISABLE_COPY(qtThrobberInterface)
};

#endif
