/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
    qtOverlayWidgetInterface(QObject* parent = 0)
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
