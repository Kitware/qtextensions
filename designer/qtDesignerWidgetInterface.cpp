/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtDesignerWidgetInterface.h"

//-----------------------------------------------------------------------------
qtDesignerWidgetInterface::qtDesignerWidgetInterface(QObject* parent)
    : QObject(parent)
{
}

//-----------------------------------------------------------------------------
bool qtDesignerWidgetInterface::isInitialized() const
{
    return true;
}

//-----------------------------------------------------------------------------
QString qtDesignerWidgetInterface::group() const
{
    return "qtExtensions";
}

//-----------------------------------------------------------------------------
QIcon qtDesignerWidgetInterface::icon() const
{
    // TODO
    return QIcon();
}

//-----------------------------------------------------------------------------
QString qtDesignerWidgetInterface::whatsThis() const
{
    return QString();
}

//-----------------------------------------------------------------------------
bool qtDesignerWidgetInterface::isContainer() const
{
    return false;
}

//-----------------------------------------------------------------------------
QString qtDesignerWidgetInterface::includeFile() const
{
    return QString("%1.h").arg(name());
}

//-----------------------------------------------------------------------------
QString qtDesignerWidgetInterface::defaultObjectName() const
{
    auto result = name();

    // Strip prefix
    while (result[0].isLower())
        result = result.mid(1);

    // Convert to initial-lowercase
    result[0] = result[0].toLower();

    // Return modified name
    return result;
}

//-----------------------------------------------------------------------------
QString qtDesignerWidgetInterface::domXml() const
{
    QDomDocument doc;
    qtDomElement root(doc, "widget");

    root.setAttribute("class", name());
    root.setAttribute("name", defaultObjectName());

    buildXml(doc, root);
    doc.appendChild(root);

    return doc.toString();
}

//-----------------------------------------------------------------------------
void qtDesignerWidgetInterface::buildXml(
    QDomDocument& document, qtDomElement& root) const
{
    Q_UNUSED(document)
    Q_UNUSED(root)
}
