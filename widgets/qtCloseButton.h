/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtCloseButton_h
#define __qtCloseButton_h

#include <QToolButton>

#include "../core/qtGlobal.h"

class qtCloseButtonPrivate;

class QTE_EXPORT qtCloseButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(bool visibleWhenDisabled
               READ visibleWhenDisabled
               WRITE setVisibleWhenDisabled)

public:
    qtCloseButton(QWidget* parent = 0);
    ~qtCloseButton();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    bool visibleWhenDisabled() const;

public slots:
    void setVisibleWhenDisabled(bool);

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtCloseButton)

    bool event(QEvent *event) override;
    void paintEvent(QPaintEvent*) override;

private:
    QTE_DECLARE_PRIVATE(qtCloseButton)
    QTE_DISABLE_COPY(qtCloseButton)
};

#endif
