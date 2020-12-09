// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDesignerPlugin_h
#define __qtDesignerPlugin_h

#include <QtDesigner/QtDesigner>

#include <QtPlugin>

#include "../core/qtGlobal.h"

class qtDesignerPlugin : public QObject,
                         public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
    Q_PLUGIN_METADATA(IID "com.kitware.qtExtensions.qtDesignerPlugin"
                      FILE "qtDesignerPluginMetaData.json")

public:
    typedef QList<QDesignerCustomWidgetInterface*> InterfaceList;
    qtDesignerPlugin(QObject* parent = nullptr);

    virtual InterfaceList customWidgets() const QTE_OVERRIDE;

protected:
    InterfaceList m_widgets;

private:
    QTE_DISABLE_COPY(qtDesignerPlugin)
};

#endif
