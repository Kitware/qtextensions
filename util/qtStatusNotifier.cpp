// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtStatusNotifier.h"

#include "qtStatusManager.h"

QTE_IMPLEMENT_D_FUNC(qtStatusNotifier)

//-----------------------------------------------------------------------------
class qtStatusNotifierPrivate
{
public:
  qtStatusNotifierPrivate(qtStatusNotifier* q) : statusSource(q) {}

  qtStatusSource statusSource;
};

//-----------------------------------------------------------------------------
qtStatusNotifier::qtStatusNotifier()
  : d_ptr(new qtStatusNotifierPrivate(this))
{
}

//-----------------------------------------------------------------------------
qtStatusNotifier::~qtStatusNotifier()
{
}

//-----------------------------------------------------------------------------
void qtStatusNotifier::addReceiver(qtStatusManager* manager) const
{
  connect(this, SIGNAL(statusMessageAvailable(qtStatusSource, QString)),
          manager, SLOT(setStatusText(qtStatusSource, QString)));
  connect(this, SIGNAL(progressAvailable(qtStatusSource, bool, qreal)),
          manager, SLOT(setProgress(qtStatusSource, bool, qreal)));
  connect(this, SIGNAL(progressAvailable(qtStatusSource, bool, int, int)),
          manager, SLOT(setProgress(qtStatusSource, bool, int, int)));
}

//-----------------------------------------------------------------------------
qtStatusSource& qtStatusNotifier::statusSource()
{
  QTE_D(qtStatusNotifier);
  return d->statusSource;
}

//-----------------------------------------------------------------------------
void qtStatusNotifier::postStatus(QString message, bool clearProgress)
{
  if (clearProgress)
    {
    emit this->progressAvailable(this->statusSource());
    }
  emit this->statusMessageAvailable(this->statusSource(), message);
}

//-----------------------------------------------------------------------------
void qtStatusNotifier::postStatus(QString message, qreal progress)
{
  emit this->statusMessageAvailable(this->statusSource(), message);
  emit this->progressAvailable(this->statusSource(), true, progress);
}

//-----------------------------------------------------------------------------
void qtStatusNotifier::postStatus(QString message, int progressValue,
                                  int progressSteps)
{
  emit this->statusMessageAvailable(this->statusSource(), message);
  emit this->progressAvailable(this->statusSource(), true,
                               progressValue, progressSteps);
}

//-----------------------------------------------------------------------------
void qtStatusNotifier::clearStatus()
{
  emit this->statusMessageAvailable(this->statusSource());
}
