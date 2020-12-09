// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
    qtDoubleSliderInterface(QObject* parent = nullptr)
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
