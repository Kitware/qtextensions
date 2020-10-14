// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtDoubleSlider.h"

#include <QApplication>
#include <QHoverEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionSlider>
#include <QWheelEvent>

#include "../core/qtMath.h"

QTE_IMPLEMENT_D_FUNC(qtDoubleSlider)

//BEGIN qtDoubleSliderPrivate

//-----------------------------------------------------------------------------
class qtDoubleSliderPrivate
{
public:
  explicit qtDoubleSliderPrivate(qtDoubleSlider* q) : q_ptr(q) {}

  void setValue(double val);

  double valueFromPosition(const QPoint& pos) const;

  QStyle::SubControl hitTest(const QPoint& pos) const;
  bool isAbsoluteSetButton(Qt::MouseButtons) const;
  bool isPageSetButton(Qt::MouseButtons) const;

  qtDoubleSlider::QuantizeMode qmode;
  double min, max, value, sStep, pStep, quant, iRange;
  int iMin, iMax;
  QStyle::SubControl pressedControl, hoverControl;
  double wRVal, wDelta;

protected:
  QTE_DECLARE_PUBLIC_PTR(qtDoubleSlider)

private:
  QTE_DECLARE_PUBLIC(qtDoubleSlider)
};

//-----------------------------------------------------------------------------
void qtDoubleSliderPrivate::setValue(double val)
{
  // Quantize value
  if (!qFuzzyIsNull(this->quant))
    {
    const double qo =
      (this->qmode == qtDoubleSlider::QuantizeRelative ? this->min : 0.0);
    const double qval =
      qo + (round((val - qo) / this->quant) * this->quant);
    const double qvDist = fabs(qval - val);

    // Round to the nearest of { min, max, qval }
    if ((val - this->min) < qvDist)
      {
      val = this->min;
      }
    else if ((this->max - val) < qvDist)
      {
      val = this->max;
      }
    else
      {
      val = qval;
      }
    }

  // Bound value to range
  val = qBound(this->min, val, this->max);

  // Update value, if changed
  if (!qFuzzyCompare(val, this->value))
    {
    QTE_Q(qtDoubleSlider);

    this->value = val;
    q->update();
    emit q->valueChanged(val);
    }
}

//-----------------------------------------------------------------------------
double qtDoubleSliderPrivate::valueFromPosition(const QPoint& pos) const
{
  const double nval = (pos.x() - this->iMin) / this->iRange;
  return this->min + (nval * (this->max - this->min));
}

//-----------------------------------------------------------------------------
QStyle::SubControl qtDoubleSliderPrivate::hitTest(const QPoint& pos) const
{
  QTE_Q_CONST(qtDoubleSlider);

  QStyleOptionSlider opt;
  q->initStyleOption(&opt);
  return q->style()->hitTestComplexControl(QStyle::CC_Slider, &opt, pos, q);
}

//-----------------------------------------------------------------------------
bool qtDoubleSliderPrivate::isAbsoluteSetButton(Qt::MouseButtons b) const
{
  QTE_Q_CONST(qtDoubleSlider);
  auto const bi = static_cast<int>(b);
  auto const sh = q->style()->styleHint(QStyle::SH_Slider_AbsoluteSetButtons);
  return ((sh & bi) == bi);
}

//-----------------------------------------------------------------------------
bool qtDoubleSliderPrivate::isPageSetButton(Qt::MouseButtons b) const
{
  QTE_Q_CONST(qtDoubleSlider);
  auto const bi = static_cast<int>(b);
  int sh = q->style()->styleHint(QStyle::SH_Slider_PageSetButtons);
  return ((sh & bi) == bi);
}

//END qtDoubleSliderPrivate

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtDoubleSlider ctor/dtor

//-----------------------------------------------------------------------------
qtDoubleSlider::qtDoubleSlider(QWidget* parent)
  : QWidget(parent), d_ptr(new qtDoubleSliderPrivate(this))
{
  QTE_D(qtDoubleSlider);
  d->min = 0.0;
  d->max = 1.0;
  d->value = 0.0;
  d->sStep = 0.01;
  d->pStep = 0.1;
  d->quant = 0.0;
  d->qmode = QuantizeAbsolute;
  d->iMin = 0;
  d->iMax = 1;
  d->iRange = 1.0;
  d->wRVal = qQNaN();
  d->wDelta = 0.0;

  d->pressedControl = QStyle::SC_None;
  d->hoverControl = QStyle::SC_None;

  int focusPolicyHint =
    this->style()->styleHint(QStyle::SH_Button_FocusPolicy);
  QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed,
                         QSizePolicy::Slider);

  this->setAttribute(Qt::WA_Hover);
  this->setFocusPolicy(Qt::StrongFocus);
  this->setFocusPolicy(static_cast<Qt::FocusPolicy>(focusPolicyHint));
  this->setSizePolicy(sizePolicy);
}

//-----------------------------------------------------------------------------
qtDoubleSlider::~qtDoubleSlider()
{
}

//END qtDoubleSlider ctor/dtor

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtDoubleSlider data interface (accessors)

//-----------------------------------------------------------------------------
double qtDoubleSlider::minimum() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->min;
}

//-----------------------------------------------------------------------------
double qtDoubleSlider::maximum() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->max;
}

//-----------------------------------------------------------------------------
double qtDoubleSlider::singleStep() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->sStep;
}

//-----------------------------------------------------------------------------
double qtDoubleSlider::pageStep() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->pStep;
}

//-----------------------------------------------------------------------------
double qtDoubleSlider::quantize() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->quant;
}

//-----------------------------------------------------------------------------
qtDoubleSlider::QuantizeMode qtDoubleSlider::quantizeMode() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->qmode;
}

//-----------------------------------------------------------------------------
double qtDoubleSlider::value() const
{
  QTE_D_CONST(qtDoubleSlider);
  return d->value;
}

//END qtDoubleSlider data interface (accessors)

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtDoubleSlider data interface (manipulators)

//-----------------------------------------------------------------------------
void qtDoubleSlider::setMinimum(double min)
{
  QTE_D(qtDoubleSlider);
  this->setRange(min, d->max);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setMaximum(double max)
{
  QTE_D(qtDoubleSlider);
  this->setRange(d->min, max);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setRange(double min, double max)
{
  if (!(qIsFinite(min) && qIsFinite(max)))
    {
    return;
    }

  min = qMin(min, max);
  max = qMax(min, max);

  QTE_D(qtDoubleSlider);
  if (!(qFuzzyCompare(d->min, min) && qFuzzyCompare(d->max, max)))
    {
    // Update range
    d->min = min;
    d->max = max;
    this->update();
    emit this->rangeChanged(min, max);

    // Check if value needs to be adjusted to remain in range
    const double boundValue = qBound(min, d->value, max);
    if (!qFuzzyCompare(d->value, boundValue))
      {
      this->setValue(boundValue);
      }
    }
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setSingleStep(double ss)
{
  if (!qIsFinite(ss))
    {
    return;
    }

  QTE_D(qtDoubleSlider);
  d->sStep = ss;
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setPageStep(double ps)
{
  if (!qIsFinite(ps))
    {
    return;
    }

  QTE_D(qtDoubleSlider);
  d->pStep = ps;
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setQuantize(double quant)
{
  QTE_D(qtDoubleSlider);
  this->setQuantize(quant, d->qmode);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setQuantize(double quant, QuantizeMode qmode)
{
  if (!qIsFinite(quant))
    {
    return;
    }

  QTE_D(qtDoubleSlider);
  d->quant = quant;
  d->qmode = qmode;
  this->setValue(d->value);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setQuantizeMode(QuantizeMode qmode)
{
  QTE_D(qtDoubleSlider);
  this->setQuantize(d->quant, qmode);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::setValue(double val)
{
  if (!qIsFinite(val))
    {
    return;
    }

  QTE_D(qtDoubleSlider);

  if (!qFuzzyCompare(val, d->value))
    {
    // If a request (either external, or internal not associated with a wheel
    // event) to change the value occurs, and the request is not to set the
    // value to the current value, then invalidate the relative value for
    // accumulating change from wheel events
    d->wRVal = qQNaN();

    // Go ahead and try to change the value (might still have no effect due to
    // bounds and/or quantizing)
    d->setValue(val);
    }
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::triggerAction(qtDoubleSlider::SliderAction action)
{
  QTE_D(qtDoubleSlider);

  double val = d->value;

  switch (action)
    {
    case QAbstractSlider::SliderSingleStepAdd:
      val += d->sStep;
      break;
    case QAbstractSlider::SliderSingleStepSub:
      val -= d->sStep;
      break;
    case QAbstractSlider::SliderPageStepAdd:
      val += d->pStep;
      break;
    case QAbstractSlider::SliderPageStepSub:
      val -= d->pStep;
      break;
    case QAbstractSlider::SliderToMinimum:
      val = d->min;
      break;
    case QAbstractSlider::SliderToMaximum:
      val = d->max;
      break;
    default:
      return;
    }
  emit this->actionTriggered(action);
  this->setValue(val);
}

//END qtDoubleSlider data interface (manipulators)

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtDoubleSlider painting

//-----------------------------------------------------------------------------
void qtDoubleSlider::initStyleOption(QStyleOptionSlider* opt) const
{
  QTE_D_CONST(qtDoubleSlider);

  // Size/position information
  opt->initFrom(this);
  opt->orientation = Qt::Horizontal;
  opt->subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
  opt->tickPosition = QSlider::NoTicks;
  opt->upsideDown = false;

  // Value information
  const double range = (d->max - d->min);
  const double nval = (range > 0.0 ? (d->value - d->min) / range : 0.0);
  opt->minimum = d->iMin;
  opt->maximum = d->iMax;
  opt->sliderValue = d->iMin + qRound(nval * d->iRange);
  opt->sliderPosition = opt->sliderValue;

  // Current state information
  if (d->pressedControl)
    {
    opt->activeSubControls = d->pressedControl;
    opt->state |= QStyle::State_Sunken;
    }
  else
    {
    opt->activeSubControls = d->hoverControl;
    }
}

//-----------------------------------------------------------------------------
QSize qtDoubleSlider::sizeHint() const
{
  QStyleOptionSlider opt;
  this->initStyleOption(&opt);

  QStyle* s = this->style();
  QSize size(64, s->pixelMetric(QStyle::PM_SliderThickness, &opt, this));
#ifndef Q_OS_MAC
  return s->sizeFromContents(QStyle::CT_Slider, &opt, size, this);
#else
  // QMacStyle::sizeFromContents is broken... rather than using the
  // QStyleOptionSlider to check attributes such as orientation, tick
  // information, etc., it tries to qobject_cast the (optional!) widget pointer
  // to a QSlider (which we are not), and proceeds to test the orientation
  // without checking if the cast succeeded. Since SEGV's are not nice, just
  // punt on trying to get the 'correct' size hint on OS/X.
  return size;
#endif
}

//-----------------------------------------------------------------------------
QSize qtDoubleSlider::minimumSizeHint() const
{
  QStyleOptionSlider opt;
  this->initStyleOption(&opt);

  return {this->style()->pixelMetric(QStyle::PM_SliderLength, &opt, this),
          this->style()->pixelMetric(QStyle::PM_SliderThickness, &opt, this)};
}

//-----------------------------------------------------------------------------
bool qtDoubleSlider::event(QEvent* e)
{
  switch (e->type())
    {
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
      {
      QTE_D(qtDoubleSlider);

      const QHoverEvent* he = static_cast<const QHoverEvent*>(e);
      QStyle::SubControl hc = d->hitTest(he->pos());

      if (hc != d->hoverControl)
        {
        d->hoverControl = hc;
        this->update();
        }
      break;
      }
    default:
      break;
    }

  return QWidget::event(e);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::paintEvent(QPaintEvent*)
{
  QPainter p(this);
  QStyleOptionSlider opt;
  this->initStyleOption(&opt);
  this->style()->drawComplexControl(QStyle::CC_Slider, &opt, &p, this);
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::resizeEvent(QResizeEvent* e)
{
  QTE_D(qtDoubleSlider);

  QStyleOptionSlider opt;
  this->initStyleOption(&opt);

  QRect scr = this->style()->subControlRect(
    QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
  d->iMin = scr.left();
  d->iMax = scr.right();
  d->iRange = (d->iMax - d->iMin);

  QWidget::resizeEvent(e);
}

//END qtDoubleSlider painting

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtDoubleSlider data manipulation event handlers

//-----------------------------------------------------------------------------
void qtDoubleSlider::keyPressEvent(QKeyEvent* e)
{
  SliderAction action = QAbstractSlider::SliderNoAction;
  switch (e->key())
    {
    case Qt::Key_Up:
    case Qt::Key_Right:
      action = QAbstractSlider::SliderSingleStepAdd;
      break;
    case Qt::Key_Down:
    case Qt::Key_Left:
      action = QAbstractSlider::SliderSingleStepSub;
      break;
    case Qt::Key_PageUp:
      action = QAbstractSlider::SliderPageStepAdd;
      break;
    case Qt::Key_PageDown:
      action = QAbstractSlider::SliderPageStepSub;
      break;
    case Qt::Key_Home:
      action = QAbstractSlider::SliderToMinimum;
      break;
    case Qt::Key_End:
      action = QAbstractSlider::SliderToMaximum;
      break;
    default:
      e->ignore();
      break;
    }

  if (action)
    {
    this->triggerAction(action);
    e->accept();
    }
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::mousePressEvent(QMouseEvent* e)
{
  const Qt::MouseButton b = e->button();
  if (e->buttons() ^ b)
    {
    e->ignore();
    return;
    }

  QTE_D(qtDoubleSlider);

  if (d->isAbsoluteSetButton(b))
    {
    e->accept();
    emit this->actionTriggered(QAbstractSlider::SliderMove);
    this->setValue(d->valueFromPosition(e->pos()));
    }
  else if (d->isPageSetButton(b))
    {
    d->pressedControl = d->hitTest(e->pos());
    switch (d->pressedControl)
      {
      case QStyle::SC_SliderGroove:
        if (d->valueFromPosition(e->pos()) > d->value)
          {
          this->triggerAction(QAbstractSlider::SliderPageStepAdd);
          }
        else
          {
          this->triggerAction(QAbstractSlider::SliderPageStepSub);
          }
        e->accept();
        break;
      case QStyle::SC_SliderHandle:
        d->wRVal = qQNaN();
        e->accept();
        this->update();
        break;
      default:
        e->ignore();
        break;
      }
    }
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::mouseReleaseEvent(QMouseEvent* e)
{
  QTE_D(qtDoubleSlider);

  e->ignore();
  if (d->isPageSetButton(e->button()))
    {
    d->pressedControl = QStyle::SC_None;
    this->update();
    e->accept();
    }
  else if (d->isAbsoluteSetButton(e->button()))
    {
    e->accept();
    }
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::mouseMoveEvent(QMouseEvent* e)
{
  QTE_D(qtDoubleSlider);

  e->ignore();
  if (d->isAbsoluteSetButton(e->buttons()))
    {
    emit this->actionTriggered(QAbstractSlider::SliderMove);
    this->setValue(d->valueFromPosition(e->pos()));
    e->accept();
    }
  else if (d->pressedControl == QStyle::SC_SliderHandle &&
           d->isPageSetButton(e->buttons()))
    {
    emit this->actionTriggered(QAbstractSlider::SliderMove);
    this->setValue(d->valueFromPosition(e->pos()));
    e->accept();
    }
}

//-----------------------------------------------------------------------------
void qtDoubleSlider::wheelEvent(QWheelEvent* e)
{
  if (!e->delta())
    {
    return;
    }

  QTE_D(qtDoubleSlider);

  // Calculate the amount by which to change the value
  const double step =
    ((e->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier))
      ? d->pStep : d->sStep * QApplication::wheelScrollLines());
  const double scroll = e->delta();
  const double delta = scroll * step / 120.0;

  // Set the relative position, if it is unset
  if (!qIsFinite(d->wRVal))
    {
    d->wRVal = d->value;
    d->wDelta = 0.0;
    }

  // Modify the value, capped to the slider's range
  d->wDelta += delta;
  if (d->wRVal + d->wDelta < d->min)
    {
    d->wRVal = d->min;
    d->wDelta = 0.0;
    }
  if (d->wRVal + d->wDelta > d->max)
    {
    d->wRVal = d->max;
    d->wDelta = 0.0;
    }
  emit this->actionTriggered(QAbstractSlider::SliderMove);
  d->setValue(d->wRVal + d->wDelta);
  e->accept();
}

//END qtDoubleSlider data manipulation event handlers
