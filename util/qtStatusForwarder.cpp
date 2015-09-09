/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
