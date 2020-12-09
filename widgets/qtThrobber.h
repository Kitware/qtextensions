// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtThrobber_h
#define __qtThrobber_h

#include <QWidget>

#include "../core/qtGlobal.h"

class qtThrobberPrivate;

class QTE_EXPORT qtThrobber : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool active READ isActive WRITE setActive)
    Q_PROPERTY(int maxThrobberSize
               READ maxThrobberSize
               WRITE setMaxThrobberSize)
    Q_PROPERTY(Style style READ style WRITE setStyle)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    enum Style
    {
        BlackAndWhite,
        TranslucentForeground,
    };
    Q_ENUM(Style)

    qtThrobber(QWidget* parent = nullptr);
    virtual ~qtThrobber();

    QSize minimumSizeHint() const;
    bool isActive() const;
    Style style() const;
    qreal opacity() const;
    int maxThrobberSize() const;

signals:
    void toggled(bool);

public slots:
    void setActive(bool);
    void setStyle(Style);
    void setOpacity(qreal);
    void setMaxThrobberSize(int);

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtThrobber)

    void paintEvent(QPaintEvent*);

private:
    QTE_DECLARE_PRIVATE(qtThrobber)
    QTE_DISABLE_COPY(qtThrobber)
};

#endif
