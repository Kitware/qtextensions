/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtCloseButtonInterface_h
#define __qtCloseButtonInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtCloseButton.h"

class qtCloseButtonInterface
    : public qtDesignerWidgetInterfaceHelper<qtCloseButton>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtCloseButtonInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtCloseButton>(parent)
    {
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A styled tool button that shows a window close button";
    }

private:
    QTE_DISABLE_COPY(qtCloseButtonInterface)
};

#endif
