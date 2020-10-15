// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtSvgWidgetInterface_h
#define __qtSvgWidgetInterface_h

#include "../widgets/qtSvgWidget.h"

#include "qtDesignerWidgetInterface.h"

class qtSvgWidgetInterface
    : public qtDesignerWidgetInterfaceHelper<qtSvgWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtSvgWidgetInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtSvgWidget>(parent)
    {
    }

    virtual bool isContainer() const QTE_OVERRIDE
    {
        return true;
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A widget that renders an SVG resource";
    }

private:
    QTE_DISABLE_COPY(qtSvgWidgetInterface)
};

#endif
