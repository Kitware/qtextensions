/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
