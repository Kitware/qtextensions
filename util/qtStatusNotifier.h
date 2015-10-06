/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtStatusNotifier_h
#define __qtStatusNotifier_h

#include <QObject>

#include "qtStatusSource.h"

class qtStatusManager;

class qtStatusNotifierPrivate;

class QTE_EXPORT qtStatusNotifier : public QObject
{
  Q_OBJECT

public:
  qtStatusNotifier();
  virtual ~qtStatusNotifier();

  void addReceiver(qtStatusManager*) const;

signals:
  void statusMessageAvailable(qtStatusSource, QString = QString());
  void progressAvailable(qtStatusSource, bool = false, qreal value = -1);
  void progressAvailable(qtStatusSource, bool, int value, int steps);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtStatusNotifier)

  qtStatusSource& statusSource();

  void postStatus(QString message, bool clearProgress = false);
  void postStatus(QString message, qreal progress);
  void postStatus(QString message, int progressValue, int progressSteps);

  void clearStatus();

private:
  QTE_DECLARE_PRIVATE(qtStatusNotifier)
  QTE_DISABLE_COPY(qtStatusNotifier)
};

#endif // __qtStatusNotifier_h
