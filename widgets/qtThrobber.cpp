/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtThrobber.h"

#include "../core/qtIndexRange.h"

#include <QPainter>
#include <QTimer>

#include <cmath>

QTE_IMPLEMENT_D_FUNC(qtThrobber)

//-----------------------------------------------------------------------------
class qtThrobberPrivate
{
public:
    constexpr static auto dotCount = 12;

    QTimer timer;
    int step = -1;
    int maxSize = -1;
    qreal opacity = 0.0;
    qtThrobber::Style style = qtThrobber::BlackAndWhite;

    QBrush brushAt(QPalette const& palette, int pos,
                   QPalette::ColorGroup group, QPalette::ColorRole role);
};

//-----------------------------------------------------------------------------
QBrush qtThrobberPrivate::brushAt(
    QPalette const& palette, int pos,
    QPalette::ColorGroup group, QPalette::ColorRole role)
{
    constexpr auto k = qtThrobberPrivate::dotCount;
    constexpr auto ki = qreal{1.0} / static_cast<qreal>(k - 1);

    auto a = static_cast<qreal>((k + pos - this->step) % k) * ki;
    a = std::pow(a, 2.2);

    if (this->style == qtThrobber::TranslucentForeground)
    {
        auto c = palette.color(group, role);
        c.setAlphaF(a);
        return c;
    }

    a = 0.1 + (0.8 * a);
    return QColor::fromRgbF(a, a, a);
}

//-----------------------------------------------------------------------------
qtThrobber::qtThrobber(QWidget* parent)
    : QWidget(parent), d_ptr(new qtThrobberPrivate)
{
    QTE_D();

    this->setBackgroundRole(QPalette::Window);
    this->setForegroundRole(QPalette::WindowText);

    connect(
        &d->timer, &QTimer::timeout, this,
        [this, d]{
            d->step = (d->step + 1) % qtThrobberPrivate::dotCount;
            this->update();
        });

    d->timer.setInterval(120);
}

//-----------------------------------------------------------------------------
qtThrobber::~qtThrobber()
{
}

//-----------------------------------------------------------------------------
QSize qtThrobber::minimumSizeHint() const
{
    return {12, 12};
}

//-----------------------------------------------------------------------------
void qtThrobber::setActive(bool active)
{
    QTE_D();
    if (active)
    {
        d->step = 0;
        d->timer.start();
    }
    else
    {
        d->timer.stop();
        d->step = -1;
        this->update();
    }
}

//-----------------------------------------------------------------------------
bool qtThrobber::isActive() const
{
    QTE_D();
    return d->timer.isActive();
}

//-----------------------------------------------------------------------------
void qtThrobber::setStyle(Style s)
{
    QTE_D();
    d->style = s;
    this->update();
}

//-----------------------------------------------------------------------------
qtThrobber::Style qtThrobber::style() const
{
    QTE_D();
    return d->style;
}

//-----------------------------------------------------------------------------
void qtThrobber::setOpacity(qreal o)
{
    QTE_D();
    d->opacity = o;
    this->update();
}

//-----------------------------------------------------------------------------
qreal qtThrobber::opacity() const
{
    QTE_D();
    return d->opacity;
}

//-----------------------------------------------------------------------------
void qtThrobber::setMaxThrobberSize(int s)
{
    QTE_D();
    d->maxSize = s;
    this->update();
}

//-----------------------------------------------------------------------------
int qtThrobber::maxThrobberSize() const
{
    QTE_D();
    return d->maxSize;
}

//-----------------------------------------------------------------------------
void qtThrobber::paintEvent(QPaintEvent* e)
{
    QTE_D();

    // Do nothing if inactive
    if (d->step < 0)
    {
        QWidget::paintEvent(e);
        return;
    }

    qreal x = this->width() * 0.5, y = this->height() * 0.5;
    qreal s = qMin(x, y), m = d->maxSize;
    const qreal r = 3.0;

    if (m > 0)
    {
        s = qMin(s, m);
    }

    QPainter painter(this);
    auto palette = this->palette();

    auto const active = this->isActiveWindow();
    auto const cg = (active ? QPalette::Active : QPalette::Inactive);
    auto const cr = this->foregroundRole();

    if (d->opacity > 0.0)
    {
        auto c = palette.color(cg, this->backgroundRole());
        c.setAlphaF(d->opacity);
        painter.fillRect(this->rect(), c);
    }

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(x, y);
    painter.scale(s * 0.04, s * 0.04);

    painter.setPen(Qt::NoPen);

    for (auto const i : qtIndexRange(qtThrobberPrivate::dotCount))
    {
        painter.save();
        painter.setBrush(d->brushAt(palette, i, cg, cr));
        painter.rotate((360.0 / qtThrobberPrivate::dotCount) * i);
        painter.drawEllipse(QRectF{-20.0 - r, -r, 2.0 * r, 2.0 * r});
        painter.restore();
    }
}
