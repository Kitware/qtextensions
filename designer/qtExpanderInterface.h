// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
    qtExpanderInterface(QObject* parent = nullptr)
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
