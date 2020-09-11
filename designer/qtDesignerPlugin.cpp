/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtDesignerPlugin.h"

#include "qtCloseButtonInterface.h"
#include "qtColorButtonInterface.h"
#include "qtDoubleSliderInterface.h"
#include "qtExpanderInterface.h"
#include "qtOverlayWidgetInterface.h"
#include "qtSqueezedLabelInterface.h"
#include "qtThrobberInterface.h"

//-----------------------------------------------------------------------------
qtDesignerPlugin::qtDesignerPlugin(QObject* parent) : QObject(parent)
{
    m_widgets.append(new qtCloseButtonInterface(this));
    m_widgets.append(new qtColorButtonInterface(this));
    m_widgets.append(new qtDoubleSliderInterface(this));
    m_widgets.append(new qtExpanderInterface(this));
    m_widgets.append(new qtOverlayWidgetInterface(this));
    m_widgets.append(new qtSqueezedLabelInterface(this));
    m_widgets.append(new qtThrobberInterface(this));
}

//-----------------------------------------------------------------------------
qtDesignerPlugin::InterfaceList qtDesignerPlugin::customWidgets() const
{
    return m_widgets;
}
