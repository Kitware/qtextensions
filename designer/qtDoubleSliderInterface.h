/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDoubleSliderInterface_h
#define __qtDoubleSliderInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtDoubleSlider.h"

class qtDoubleSliderInterface
    : public qtDesignerWidgetInterfaceHelper<qtDoubleSlider>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtDoubleSliderInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtDoubleSlider>(parent)
    {
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A slider that uses floating point values";
    }

private:
    QTE_DISABLE_COPY(qtDoubleSliderInterface)
};

#endif
