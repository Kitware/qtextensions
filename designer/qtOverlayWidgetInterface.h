// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtOverlayWidgetInterface_h
#define __qtOverlayWidgetInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtOverlayWidget.h"

class qtOverlayWidgetInterface
    : public qtDesignerWidgetInterfaceHelper<qtOverlayWidget>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtOverlayWidgetInterface(QObject* parent = nullptr)
        : qtDesignerWidgetInterfaceHelper<qtOverlayWidget>(parent)
    {
    }

    virtual bool isContainer() const QTE_OVERRIDE
    {
        return true;
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A container widget that overlays other widgets";
    }

private:
    QTE_DISABLE_COPY(qtOverlayWidgetInterface)
};

#endif
