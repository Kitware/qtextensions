// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QEvent>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionToolButton>

#include "qtCloseButton.h"

//-----------------------------------------------------------------------------
class qtCloseButtonPrivate
{
public:
    void updatePixmap(QStyle* style);

    QPixmap pixmap;
    bool visibleWhenDisabled = true;
};
QTE_IMPLEMENT_D_FUNC(qtCloseButton)

//-----------------------------------------------------------------------------
void qtCloseButtonPrivate::updatePixmap(QStyle *style)
{
    auto const size =
        style->pixelMetric(QStyle::PM_TitleBarButtonIconSize);
    auto const& icon =
        style->standardIcon(QStyle::SP_TitleBarCloseButton);
    this->pixmap = icon.pixmap({size, size});
}

//-----------------------------------------------------------------------------
qtCloseButton::qtCloseButton(QWidget* parent)
  : QToolButton{parent}, d_ptr{new qtCloseButtonPrivate}
{
    this->setAutoRaise(true);
    this->setCheckable(false);
}

//-----------------------------------------------------------------------------
qtCloseButton::~qtCloseButton()
{
}

//-----------------------------------------------------------------------------
bool qtCloseButton::visibleWhenDisabled() const
{
    QTE_D();
    return d->visibleWhenDisabled;
}

//-----------------------------------------------------------------------------
void qtCloseButton::setVisibleWhenDisabled(bool state)
{
    QTE_D();

    d->visibleWhenDisabled = state;

    if (!this->isEnabled())
    {
        this->update();
    }
}

//-----------------------------------------------------------------------------
QSize qtCloseButton::sizeHint() const
{
    return qtCloseButton::minimumSizeHint();
}

//-----------------------------------------------------------------------------
QSize qtCloseButton::minimumSizeHint() const
{
    auto const iconSize =
        this->style()->pixelMetric(QStyle::PM_TitleBarButtonIconSize);
    auto const frameSize =
        this->style()->pixelMetric(QStyle::PM_ToolBarFrameWidth);
    auto const totalSize = iconSize + (2 * frameSize);
    return {totalSize, totalSize};
}

//-----------------------------------------------------------------------------
bool qtCloseButton::event(QEvent *event)
{
    QTE_D();

    auto const result = QWidget::event(event);

    switch (event->type())
    {
        case QEvent::ApplicationPaletteChange:
        case QEvent::PaletteChange:
        case QEvent::StyleChange:
            d->updatePixmap(this->style());
            break;

        default:
            break;
    }

    return result;
}

//-----------------------------------------------------------------------------
void qtCloseButton::paintEvent(QPaintEvent*)
{
    QTE_D();

    // Initialize drawing options
    QStylePainter p{this};
    QStyleOptionToolButton opt;
    this->initStyleOption(&opt);
    opt.text = QString{};
    opt.icon = QIcon{};

    // Hand off to the style to draw the widget base
    p.drawComplexControl(QStyle::CC_ToolButton, opt);

    // Draw close 'button'
    if (this->isEnabled() || d->visibleWhenDisabled)
    {
        if (d->pixmap.isNull())
        {
            d->updatePixmap(this->style());
        }

        auto const frameSize =
            this->style()->pixelMetric(QStyle::PM_ToolBarFrameWidth);
        p.drawPixmap(QPoint{frameSize, frameSize}, d->pixmap);
    }
}
