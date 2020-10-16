// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
