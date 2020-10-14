// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtStatusSource.h"

#include "qtEnableSharedFromThis.h"

#include <QObject>
#include <QPointer>

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
  QPointer<QObject> ownerRef;
  QString ownerIdentifier;
  QString displayIdentifier;
};
