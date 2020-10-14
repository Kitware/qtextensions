// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtSqueezedLabelInterface_h
#define __qtSqueezedLabelInterface_h

#include "qtDesignerWidgetInterface.h"

#include "../widgets/qtSqueezedLabel.h"

class qtSqueezedLabelInterface
    : public qtDesignerWidgetInterfaceHelper<qtSqueezedLabel>
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtSqueezedLabelInterface(QObject* parent = 0)
        : qtDesignerWidgetInterfaceHelper<qtSqueezedLabel>(parent)
    {
    }

    virtual bool isContainer() const QTE_OVERRIDE
    {
        return true;
    }

    virtual QString toolTip() const QTE_OVERRIDE
    {
        return "A label that squeezes its text";
    }

private:
    QTE_DISABLE_COPY(qtSqueezedLabelInterface)
};

#endif
