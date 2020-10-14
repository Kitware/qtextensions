// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
