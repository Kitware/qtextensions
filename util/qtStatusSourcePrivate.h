/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtStatusSource.h"

#include "qtEnableSharedFromThis.h"
#include "qtPointer.h"

#include <QObject>

class qtStatusSourcePrivate
    : public qtEnableSharedFromThis<qtStatusSourcePrivate, QObject>
{
  Q_OBJECT

public:
  qtStatusSourcePrivate(QObject* owner);

signals:
  void ownerDestroyed(qtStatusSource);

protected slots:
  void ownerDestroyed();

private:
  QTE_DISABLE_COPY(qtStatusSourcePrivate)

  friend class qtStatusSource;
  friend QDebug& operator<<(QDebug&, const qtStatusSource&);

  QObject* owner;
  qtPointer<QObject> ownerRef;
  QString ownerIdentifier;
  QString displayIdentifier;
};
