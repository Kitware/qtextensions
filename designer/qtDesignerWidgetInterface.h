/*ckwg +5
 * Copyright 2019 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDesignerWidgetInterface_h
#define __qtDesignerWidgetInterface_h

#include <QDesignerCustomWidgetInterface>

#include "../core/qtGlobal.h"
#include "../dom/qtDomElement.h"

class qtDesignerWidgetInterface : public QObject,
                                  public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    qtDesignerWidgetInterface(QObject* parent = 0);

    virtual bool isInitialized() const QTE_OVERRIDE;

    virtual QString domXml() const QTE_OVERRIDE;
    virtual QString group() const QTE_OVERRIDE;
    virtual QIcon icon() const QTE_OVERRIDE;
    virtual QString whatsThis() const QTE_OVERRIDE;

    virtual bool isContainer() const;

    virtual QString includeFile() const QTE_OVERRIDE;

protected:
    virtual QString defaultObjectName() const;

    virtual void buildXml(QDomDocument& document, qtDomElement& root) const;

private:
    QTE_DISABLE_COPY(qtDesignerWidgetInterface)
};

template <typename Widget>
class qtDesignerWidgetInterfaceHelper
    : public qtDesignerWidgetInterface
{
public:
    qtDesignerWidgetInterfaceHelper(QObject* parent)
        : qtDesignerWidgetInterface(parent)
    {
    }

    virtual QString name() const QTE_OVERRIDE
    {
        return Widget::staticMetaObject.className();
    }

    virtual QWidget* createWidget(QWidget* parent) QTE_OVERRIDE
    {
        return new Widget(parent);
    }
};

#endif
