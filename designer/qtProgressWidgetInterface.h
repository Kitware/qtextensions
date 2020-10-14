// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtProgressWidgetInterface_h
#define __qtProgressWidgetInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtProgressWidget.h"

class qtProgressWidgetInterface :
    public qtDesignerWidgetInterfaceHelper<qtProgressWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtProgressWidgetInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtProgressWidget>(parent)
    {
    }

    virtual bool isContainer() const QTE_OVERRIDE
    {
        return false;
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A container widget for reporting progress";
    }

private:
    QTE_DISABLE_COPY(qtProgressWidgetInterface)
};

#endif
