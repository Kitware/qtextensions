/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtThread.h"

QTE_IMPLEMENT_D_FUNC(qtBareThread)

//-----------------------------------------------------------------------------
class qtBareThreadPrivate : public QThread
{
public:
  qtBareThreadPrivate(qtBareThread* parent) : q_ptr(parent) {}
  virtual ~qtBareThreadPrivate() {}

  virtual void run() { QTE_Q(qtBareThread); q->run(); }

private:
  QTE_DECLARE_PUBLIC(qtBareThread)
  QTE_DECLARE_PUBLIC_PTR(qtBareThread)
};

//-----------------------------------------------------------------------------
qtBareThread::qtBareThread(QObject* self)
  : d_ptr(new qtBareThreadPrivate(this))
{
  QTE_D(qtBareThread);
  self->moveToThread(d);
}

//-----------------------------------------------------------------------------
qtBareThread::~qtBareThread()
{
  QTE_D(qtBareThread);
  delete d;
}

//-----------------------------------------------------------------------------
void qtBareThread::start()
{
  QTE_D(qtBareThread);
  d->start();
}

//-----------------------------------------------------------------------------
void qtBareThread::quit()
{
  QTE_D(qtBareThread);
  d->quit();
}

//-----------------------------------------------------------------------------
bool qtBareThread::wait(long unsigned int time)
{
  QTE_D(qtBareThread);
  return d->wait(time);
}

//-----------------------------------------------------------------------------
void qtBareThread::run()
{
  QTE_D(qtBareThread);
  d->exec();
}

//-----------------------------------------------------------------------------
bool qtBareThread::isRunning() const
{
  QTE_D_CONST(qtBareThread);
  return d->isRunning();
}

//-----------------------------------------------------------------------------
void qtBareThread::sleep(unsigned long seconds)
{
  qtBareThreadPrivate::sleep(seconds);
}

//-----------------------------------------------------------------------------
void qtBareThread::msleep(unsigned long milliseconds)
{
  qtBareThreadPrivate::msleep(milliseconds);
}

//-----------------------------------------------------------------------------
void qtBareThread::usleep(unsigned long microseconds)
{
  qtBareThreadPrivate::usleep(microseconds);
}
