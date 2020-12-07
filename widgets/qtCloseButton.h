// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
    qtCloseButton(QWidget* parent = nullptr);
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
