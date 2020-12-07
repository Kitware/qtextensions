// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
    qtCloseButtonInterface(QObject* parent = nullptr)
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
