// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
    qtColorButtonInterface(QObject* parent = nullptr)
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
