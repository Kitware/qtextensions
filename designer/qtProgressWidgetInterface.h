/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtProgressWidgetInterface_h
#define __qtProgressWidgetInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtProgressWidget.h"

class qtProgressWidgetInterface
    : public qtDesignerWidgetInterfaceHelper<qtProgressWidget>
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
        return true;
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A container widget for reporting progress";
    }

private:
    QTE_DISABLE_COPY(qtProgressWidgetInterface)
};

#endif

