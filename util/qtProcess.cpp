/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtProcess.h"

#include <QtCore/QDebug>
#include <QtCore/QEventLoop>

QTE_IMPLEMENT_D_FUNC(qtProcess)

//-----------------------------------------------------------------------------
class qtProcessPrivate : public QObject
{
public:
  QEventLoop eventLoop;
};

//-----------------------------------------------------------------------------
qtProcess::qtProcess(QObject* parent) :
  QProcess(parent),
  d_ptr(new qtProcessPrivate)
{
  QTE_D(qtProcess);

  connect(this, SIGNAL(finished(int, QProcess::ExitStatus)),
          &d->eventLoop, SLOT(quit()));
  connect(this, SIGNAL(error(QProcess::ProcessError)),
          &d->eventLoop, SLOT(quit()));
}

//-----------------------------------------------------------------------------
qtProcess::~qtProcess()
{
}

//-----------------------------------------------------------------------------
void qtProcess::join()
{
  QTE_D(qtProcess);
  if (this->state() != NotRunning)
    {
    d->eventLoop.exec();
    }
}

//-----------------------------------------------------------------------------
void qtProcess::report()
{
  // Read output from encoding process
  const auto& out = this->readAllStandardOutput();
  const auto& err = this->readAllStandardError();
  if (!out.isEmpty())
    {
    qDebug() << out.constData();
    }
  if (!err.isEmpty())
    {
    qWarning() << err.constData();
    }
}
