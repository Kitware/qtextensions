/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
