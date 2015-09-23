/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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

public:
    typedef QList<QDesignerCustomWidgetInterface*> InterfaceList;
    qtDesignerPlugin(QObject* parent = 0);

    virtual InterfaceList customWidgets() const QTE_OVERRIDE;

protected:
    InterfaceList m_widgets;

private:
    QTE_DISABLE_COPY(qtDesignerPlugin)
};

#endif
