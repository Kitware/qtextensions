// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtSvgWidget_h
#define __qtSvgWidget_h

#include "../core/qtGlobal.h"

#include <QWidget>

class qtSvgWidgetPrivate;

class QTE_EXPORT qtSvgWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString resource READ resource WRITE setResource)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool scaledContents
               READ hasScaledContents WRITE setScaledContents)

public:
    qtSvgWidget(QWidget* parent = nullptr, Qt::WindowFlags = 0);
    qtSvgWidget(QString const& resource, QWidget* parent = nullptr,
                Qt::WindowFlags = 0);
    virtual ~qtSvgWidget();

    QString resource() const;

    Qt::Alignment alignment() const;
    bool hasScaledContents() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void setResource(QString const&);
    void setAlignment(Qt::Alignment);
    void setScaledContents(bool);

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtSvgWidget)

    virtual void paintEvent(QPaintEvent*);

private:
    QTE_DECLARE_PRIVATE(qtSvgWidget)
    QTE_DISABLE_COPY(qtSvgWidget)
};

#endif
