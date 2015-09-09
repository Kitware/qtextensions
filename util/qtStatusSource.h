/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtStatusSource_h
#define __qtStatusSource_h

#include <QObject>
#include <QSharedPointer>

#include "../core/qtGlobal.h"

class QDebug;

class qtStatusSourcePrivate;

class QTE_EXPORT qtStatusSource
{
public:
  /*implicit*/ qtStatusSource(QObject* = 0);
  qtStatusSource(const qtStatusSource& other);

  ~qtStatusSource();

  void setName(QObject*);

  qtStatusSource& operator=(const qtStatusSource& other);

  bool operator==(const qtStatusSource& other) const;

protected:
  QTE_DECLARE_PRIVATE_SPTR(qtStatusSource)

  friend class qtStatusManager;
  friend class qtStatusManagerPrivate;

  friend QDebug& operator<<(QDebug&, const qtStatusSource&);

  qtStatusSource(qtStatusSourcePrivate* d);

  const QObject* owner() const;
  bool isOwnerDestroyed() const;

private:
  QTE_DECLARE_PRIVATE(qtStatusSource)
};

extern QDebug& operator<<(QDebug&, const qtStatusSource&);

#endif
