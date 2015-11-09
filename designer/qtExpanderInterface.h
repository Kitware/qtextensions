/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtExpanderInterface_h
#define __qtExpanderInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtExpander.h"

class qtExpanderInterface
    : public qtDesignerWidgetInterfaceHelper<qtExpander>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtExpanderInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtExpander>(parent)
    {
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A styled tool button that shows an expand arrow";
    }

private:
    QTE_DISABLE_COPY(qtExpanderInterface)
};

#endif
