/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <cmath>

#include "qtAbstractAnimation.h"

QTE_IMPLEMENT_D_FUNC(qtAbstractAnimation)

//-----------------------------------------------------------------------------
class qtAbstractAnimationPrivate : protected QAbstractAnimation
{
public:
  qtAbstractAnimationPrivate(qtAbstractAnimation* q)
    : QAbstractAnimation(q), q_ptr(q), absRate(1.0), dirRate(1.0) {}

protected:
  QTE_DECLARE_PUBLIC_PTR(qtAbstractAnimation)

  virtual int duration() const;
  virtual void updateCurrentTime(int currentTime);

  qreal absRate;
  qreal dirRate;

private:
  QTE_DECLARE_PUBLIC(qtAbstractAnimation)
};

//-----------------------------------------------------------------------------
int qtAbstractAnimationPrivate::duration() const
{
  QTE_Q_CONST(qtAbstractAnimation);
  return static_cast<int>(q->duration() * 1e3 / this->absRate);
}

//-----------------------------------------------------------------------------
void qtAbstractAnimationPrivate::updateCurrentTime(int currentTime)
{
  QTE_Q(qtAbstractAnimation);
  double currentTimeReal = currentTime;
  q->updateCurrentTime(currentTimeReal * 1e-3 * this->absRate);
}

//-----------------------------------------------------------------------------
qtAbstractAnimation::qtAbstractAnimation(QObject* parent)
  : QObject(parent), d_ptr(new qtAbstractAnimationPrivate(this))
{
  QTE_D(qtAbstractAnimation);
  connect(d, SIGNAL(finished()), this, SIGNAL(finished()));
  connect(d, SIGNAL(stateChanged(QAbstractAnimation::State,
                                 QAbstractAnimation::State)),
          this, SIGNAL(stateChanged(QAbstractAnimation::State,
                                    QAbstractAnimation::State)));
  connect(d, SIGNAL(currentLoopChanged(int)),
          this, SIGNAL(currentLoopChanged(int)));
}

//-----------------------------------------------------------------------------
qtAbstractAnimation::~qtAbstractAnimation()
{
}

//-----------------------------------------------------------------------------
QAbstractAnimation::State qtAbstractAnimation::state() const
{
  QTE_D_CONST(qtAbstractAnimation);
  return d->state();
}

//-----------------------------------------------------------------------------
qreal qtAbstractAnimation::rate() const
{
  QTE_D_CONST(qtAbstractAnimation);
  return d->dirRate;
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::setRate(qreal rate)
{
  QTE_D(qtAbstractAnimation);
  if (rate == d->dirRate)
    {
    return;
    }

  if (fabs(rate) != d->absRate)
    {
    double t = this->currentTime();
    d->absRate = fabs(rate);
    this->setCurrentTime(t);
    }
  d->dirRate = rate;
  d->setDirection(rate > 0 ? QAbstractAnimation::Forward
                           : QAbstractAnimation::Backward);

  emit this->rateChanged(rate);
}

//-----------------------------------------------------------------------------
double qtAbstractAnimation::currentTime() const
{
  QTE_D_CONST(qtAbstractAnimation);
  double currentTimeReal = d->currentTime();
  return currentTimeReal * 1e-3 * d->absRate;
}

//-----------------------------------------------------------------------------
double qtAbstractAnimation::currentLoopTime() const
{
  QTE_D_CONST(qtAbstractAnimation);
  double currentLoopTimeReal = d->currentLoopTime();
  return currentLoopTimeReal * 1e-3 * d->absRate;
}

//-----------------------------------------------------------------------------
int qtAbstractAnimation::loopCount() const
{
  QTE_D_CONST(qtAbstractAnimation);
  return d->loopCount();
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::setLoopCount(int loopCount)
{
  QTE_D(qtAbstractAnimation);
  d->setLoopCount(loopCount);
}

//-----------------------------------------------------------------------------
int qtAbstractAnimation::currentLoop() const
{
  QTE_D_CONST(qtAbstractAnimation);
  return d->currentLoop();
}

//-----------------------------------------------------------------------------
double qtAbstractAnimation::totalDuration() const
{
  double dur = this->duration();
  if (dur <= 0.0)
    {
    return dur;
    }
  int lc = this->loopCount();
  if (lc < 0)
    {
    return -1.0;
    }
  double lcr = lc;
  return dur * lcr;
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::start()
{
  QTE_D(qtAbstractAnimation);
  d->start();
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::pause()
{
  QTE_D(qtAbstractAnimation);
  d->pause();
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::resume()
{
  QTE_D(qtAbstractAnimation);
  d->resume();
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::setPaused(bool paused)
{
  QTE_D(qtAbstractAnimation);
  d->setPaused(paused);
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::stop()
{
  QTE_D(qtAbstractAnimation);
  d->stop();
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::setCurrentTime(double secs)
{
  QTE_D(qtAbstractAnimation);
  double currentTimeReal = d->currentTime();
  if (secs == (currentTimeReal * 1e-3 * d->absRate))
    {
    return;
    }

  d->setCurrentTime(static_cast<int>(secs * 1e3 / d->absRate));
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::updateState(QAbstractAnimation::State,
                                      QAbstractAnimation::State)
{
}

//-----------------------------------------------------------------------------
void qtAbstractAnimation::updateRate(qreal)
{
}
