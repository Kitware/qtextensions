// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
