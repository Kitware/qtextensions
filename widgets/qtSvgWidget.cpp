// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtSvgWidget.h"

#include <QPainter>
#include <QPixmapCache>
#include <QSvgRenderer>

QTE_IMPLEMENT_D_FUNC(qtSvgWidget)

//-----------------------------------------------------------------------------
class qtSvgWidgetPrivate
{
public:
    QSize adjustedSize(QSize const& in) const;

    QString resource;
    QSvgRenderer renderer;

    Qt::Alignment alignment = Qt::AlignCenter;
    bool scaled = true;

    QPixmap cachedPixmap;
};

//-----------------------------------------------------------------------------
QSize qtSvgWidgetPrivate::adjustedSize(QSize const& in) const
{
    auto const baseSize = this->renderer.defaultSize();

    if (this->scaled)
    {
        auto const bw = static_cast<double>(baseSize.width());
        auto const bh = static_cast<double>(baseSize.height());
        auto const iw = static_cast<double>(in.width());
        auto const ih = static_cast<double>(in.height());
        auto const ba = bw / bh;
        auto const ia = iw / ih;

        if (ba > ia)
        {
            auto const oh = static_cast<int>(iw / ba);
            return {in.width(), oh};
        }
        else
        {
            auto ow = static_cast<int>(ih * ba);
            return {ow, in.height()};
        }
    }

    return baseSize;
}

//-----------------------------------------------------------------------------
qtSvgWidget::qtSvgWidget(QWidget* parent, Qt::WindowFlags f)
    : QWidget{parent, f}, d_ptr{new qtSvgWidgetPrivate}
{
}

//-----------------------------------------------------------------------------
qtSvgWidget::qtSvgWidget(
    QString const& resource, QWidget* parent, Qt::WindowFlags f)
    : QWidget{parent, f}, d_ptr{new qtSvgWidgetPrivate}
{
    this->setResource(resource);
}

//-----------------------------------------------------------------------------
qtSvgWidget::~qtSvgWidget()
{
}

//-----------------------------------------------------------------------------
QString qtSvgWidget::resource() const
{
    QTE_D();
    return d->resource;
}

//-----------------------------------------------------------------------------
void qtSvgWidget::setResource(QString const& resource)
{
    QTE_D();

    if (d->resource != resource)
    {
        d->resource = resource;
        d->cachedPixmap = {};

        d->renderer.load(resource);
    }
}

//-----------------------------------------------------------------------------
Qt::Alignment qtSvgWidget::alignment() const
{
    QTE_D();
    return d->alignment;
}

//-----------------------------------------------------------------------------
void qtSvgWidget::setAlignment(Qt::Alignment alignment)
{
    QTE_D();

    if (d->alignment != alignment)
    {
        d->alignment = alignment;
        if (d->renderer.isValid())
        {
            this->update();
        }
    }
}

//-----------------------------------------------------------------------------
bool qtSvgWidget::hasScaledContents() const
{
    QTE_D();
    return d->scaled;
}

//-----------------------------------------------------------------------------
void qtSvgWidget::setScaledContents(bool scaled)
{
    QTE_D();

    if (d->scaled != scaled)
    {
        d->scaled = scaled;
        if (d->renderer.isValid())
        {
            this->update();
        }
    }
}

//-----------------------------------------------------------------------------
QSize qtSvgWidget::sizeHint() const
{
    QTE_D();
    return (d->renderer.isValid() ? d->renderer.defaultSize() : QSize{0, 0});
}

//-----------------------------------------------------------------------------
QSize qtSvgWidget::minimumSizeHint() const
{
    return this->sizeHint();
}

//-----------------------------------------------------------------------------
void qtSvgWidget::paintEvent(QPaintEvent* e)
{
    QTE_D();

    QPainter painter{this};

    if (this->autoFillBackground())
    {
        auto const role = this->backgroundRole();
        auto const group =
            [this]{
                if (!this->isEnabled()) return QPalette::Disabled;
                if (!this->isActiveWindow()) return QPalette::Inactive;
                return QPalette::Active;
            }();
        auto const& brush = this->palette().brush(group, role);

        painter.fillRect(this->rect(), brush);
    }

    if (d->renderer.isValid())
    {
        auto const size = d->adjustedSize(this->size());
        auto const dpr = this->devicePixelRatioF();
        auto const physicalSize = (QSizeF{size} * dpr).toSize();

        if (d->cachedPixmap.isNull() ||
            d->cachedPixmap.devicePixelRatioF() != dpr ||
            d->cachedPixmap.size() != physicalSize)
        {
            d->cachedPixmap = QPixmap{physicalSize};
            d->cachedPixmap.setDevicePixelRatio(dpr);
            d->cachedPixmap.fill(Qt::transparent);

            QPainter pp{&d->cachedPixmap};

            pp.setRenderHint(QPainter::Antialiasing);
            d->renderer.render(&pp, QRect{{0, 0}, size});
        }

        auto const r = this->rect();
        auto const xd = r.width() - size.width();
        auto const yd = r.height() - size.height();

        auto const a = d->alignment;
        auto const xs =
            (a & Qt::AlignRight ? 1.0 : a & Qt::AlignHCenter ? 0.5 : 0.0);
        auto const ys =
            (a & Qt::AlignBottom ? 1.0 : a & Qt::AlignVCenter ? 0.5 : 0.0);

        auto const xo = static_cast<int>(static_cast<qreal>(xd) * xs);
        auto const yo = static_cast<int>(static_cast<qreal>(yd) * ys);

        painter.drawPixmap(r.left() + xo, r.top() + yo, d->cachedPixmap);
    }
}
