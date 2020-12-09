// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  /*implicit*/ qtStatusSource(QObject* = nullptr);
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
