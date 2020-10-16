// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtGradientWidget.h"

#include "../util/qtColorUtil.h"

#include "../core/qtIndexRange.h"

#include <QImage>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>

QTE_IMPLEMENT_D_FUNC(qtGradientWidget)

//-----------------------------------------------------------------------------
class qtGradientWidgetPrivate
{
public:
  Qt::Orientation orientation;
  qtGradient gradient;
  QPixmap pixmap;
};

//-----------------------------------------------------------------------------
qtGradientWidget::qtGradientWidget(QWidget* parent)
  : QWidget(parent), d_ptr(new qtGradientWidgetPrivate)
{
  QTE_D(qtGradientWidget);
  d->orientation = Qt::Horizontal;
}

//-----------------------------------------------------------------------------
qtGradientWidget::~qtGradientWidget()
{
}

//-----------------------------------------------------------------------------
Qt::Orientation qtGradientWidget::orientation() const
{
  QTE_D_CONST(qtGradientWidget);
  return d->orientation;
}

//-----------------------------------------------------------------------------
void qtGradientWidget::setOrientation(Qt::Orientation newOrientation)
{
  QTE_D(qtGradientWidget);
  if (d->orientation != newOrientation)
    {
    d->pixmap = QPixmap();
    d->orientation = newOrientation;
    this->update();
    }
}

//-----------------------------------------------------------------------------
qtGradient qtGradientWidget::gradient() const
{
  QTE_D_CONST(qtGradientWidget);
  return d->gradient;
}

//-----------------------------------------------------------------------------
void qtGradientWidget::setGradient(qtGradient gradient)
{
  QTE_D(qtGradientWidget);
  d->gradient = gradient;
  d->pixmap = QPixmap();
  this->update();
}

//-----------------------------------------------------------------------------
QSize qtGradientWidget::minimumSizeHint() const
{
  QTE_D_CONST(qtGradientWidget);
  const bool horizontal = (d->orientation == Qt::Horizontal);
  return (horizontal ? QSize(32, 12) : QSize(12, 32));
}

//-----------------------------------------------------------------------------
QSize qtGradientWidget::sizeHint() const
{
  QTE_D_CONST(qtGradientWidget);
  const bool horizontal = (d->orientation == Qt::Horizontal);
  return (horizontal ? QSize(192, 24) : QSize(24, 192));
}

//-----------------------------------------------------------------------------
void qtGradientWidget::paintEvent(QPaintEvent*)
{
  QTE_D(qtGradientWidget);

  // Get relevant dimension of pixmap and widget
  int lastSize, currentSize;
  if (d->orientation == Qt::Horizontal)
    {
    lastSize = d->pixmap.width();
    currentSize = this->width();
    }
  else
    {
    lastSize = d->pixmap.height();
    currentSize = this->height();
    }

  // Regenerate pixmap, if needed
  if (lastSize != currentSize || d->pixmap.isNull())
    {
    QImage image(this->width(), 12, QImage::Format_RGB32);
    QColor c1 = this->palette().color(QPalette::Window);
    QColor c2 = this->palette().color(QPalette::WindowText);
    c2 = qtColorUtil::blend(c1, c2, 0.4);

    const int k = this->width();
    QList<QColor> g = d->gradient.render(k);

    foreach (auto const i, qtIndexRange(k))
      {
      const QColor& color = g[i];
      const qreal a = color.alphaF();
      const QColor cg1 = qtColorUtil::blend(c1, color, a);
      const QColor cg2 = qtColorUtil::blend(c2, color, a);
      foreach (auto const j, qtIndexRange(12))
        {
        const QColor& cb = (((i % 12) > 5) != ((j % 12) > 5) ? cg1 : cg2);
        image.setPixel(i, j, cb.rgb());
        }
      }

    d->pixmap = QPixmap::fromImage(image);
    }

  QPainter painter(this);
  painter.drawTiledPixmap(this->rect(), d->pixmap);
}
