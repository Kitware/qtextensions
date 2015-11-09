/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtColorButtonInterface_h
#define __qtColorButtonInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtColorButton.h"

class qtColorButtonInterface
    : public qtDesignerWidgetInterfaceHelper<qtColorButton>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtColorButtonInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtColorButton>(parent)
    {
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A button that shows a color swatch and"
               " allows the user to pick a color";
    }

private:
    QTE_DISABLE_COPY(qtColorButtonInterface)
};

#endif
