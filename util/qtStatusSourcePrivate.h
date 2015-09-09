/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QObject>
#include <QWeakPointer>

#include "qtStatusSource.h"

class qtStatusSourcePrivate : public QObject
{
  Q_OBJECT

public:
  qtStatusSourcePrivate(QObject* owner);

signals:
  void ownerDestroyed(qtStatusSource);

protected slots:
  void ownerDestroyed();

private:
  Q_DISABLE_COPY(qtStatusSourcePrivate)

  friend class qtStatusSource;
  friend QDebug& operator<<(QDebug&, const qtStatusSource&);

  QObject* owner;
  QWeakPointer<QObject> ownerRef;
  QString ownerIdentifier;
  QString displayIdentifier;
};
