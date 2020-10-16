// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtStatusForwarder.h"

//-----------------------------------------------------------------------------
qtStatusForwarder::qtStatusForwarder()
{
}

//-----------------------------------------------------------------------------
qtStatusForwarder::~qtStatusForwarder()
{
}

//-----------------------------------------------------------------------------
void qtStatusForwarder::connect(qtStatusNotifier* other)
{
  connect(other, SIGNAL(statusMessageAvailable(qtStatusSource, QString)),
          this, SLOT(forwardStatusMessage(qtStatusSource, QString)));
  connect(other, SIGNAL(progressAvailable(qtStatusSource, bool, qreal)),
          this, SLOT(forwardProgress(qtStatusSource, bool, qreal)));
  connect(other, SIGNAL(progressAvailable(qtStatusSource, bool, int, int)),
          this, SLOT(forwardProgress(qtStatusSource, bool, int, int)));
}

//-----------------------------------------------------------------------------
void qtStatusForwarder::forwardStatusMessage(
  qtStatusSource ss, QString message)
{
  Q_UNUSED(ss)
  emit this->statusMessageAvailable(this->statusSource(), message);
}

//-----------------------------------------------------------------------------
void qtStatusForwarder::forwardProgress(
  qtStatusSource ss, bool available, qreal progress)
{
  Q_UNUSED(ss)
  emit this->progressAvailable(this->statusSource(), available, progress);
}

//-----------------------------------------------------------------------------
void qtStatusForwarder::forwardProgress(
  qtStatusSource ss, bool available, int progressValue, int progressSteps)
{
  Q_UNUSED(ss)
  emit this->progressAvailable(this->statusSource(), available,
                               progressValue, progressSteps);
}
