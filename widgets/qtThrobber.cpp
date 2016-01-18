/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
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
  static const int dotCount = 12;

  QTimer timer;
  int step;
  int maxSize;

  QBrush brushAt(int pos);
};

//-----------------------------------------------------------------------------
QBrush qtThrobberPrivate::brushAt(int pos)
{
  qreal a = ((dotCount + pos - this->step) % dotCount);
  a = 0.1 + (0.75 * pow(a / dotCount, 2.2));
  int c = qRound(a * 255.0);
  return QColor{c, c, c};
}

//-----------------------------------------------------------------------------
qtThrobber::qtThrobber(QWidget* parent)
  : QWidget(parent), d_ptr(new qtThrobberPrivate)
{
  QTE_D(qtThrobber);
  connect(&d->timer, SIGNAL(timeout()), this, SLOT(step()));
  d->timer.setInterval(120);
  d->step = -1;
  d->maxSize = -1;
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
  QTE_D(qtThrobber);
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
  QTE_D_CONST(qtThrobber);
  return d->timer.isActive();
}

//-----------------------------------------------------------------------------
void qtThrobber::setMaxThrobberSize(int s)
{
  QTE_D(qtThrobber);
  d->maxSize = s;
}

//-----------------------------------------------------------------------------
int qtThrobber::maxThrobberSize() const
{
  QTE_D_CONST(qtThrobber);
  return d->maxSize;
}

//-----------------------------------------------------------------------------
void qtThrobber::step()
{
  QTE_D(qtThrobber);
  d->step = (d->step + 1) % qtThrobberPrivate::dotCount;
  this->update();
}

//-----------------------------------------------------------------------------
void qtThrobber::paintEvent(QPaintEvent* e)
{
  QTE_D(qtThrobber);

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

  painter.setRenderHint(QPainter::Antialiasing);
  painter.translate(x, y);
  painter.scale(s * 0.04, s * 0.04);

  painter.setPen(Qt::NoPen);

  foreach (auto const i, qtIndexRange(qtThrobberPrivate::dotCount))
    {
    painter.save();
    painter.setBrush(d->brushAt(i));
    painter.rotate((360.0 / qtThrobberPrivate::dotCount) * i);
    painter.drawEllipse(QRectF{-20.0 - r, -r, 2.0 * r, 2.0 * r});
    painter.restore();
    }
}
