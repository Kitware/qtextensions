/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QDebug>

#include "qtStatusSourcePrivate.h"

QTE_IMPLEMENT_D_FUNC(qtStatusSource)

//-----------------------------------------------------------------------------
qtStatusSourcePrivate::qtStatusSourcePrivate(QObject* obj)
  : owner(obj), ownerRef(obj)
{
  if (obj)
    {
    // Use a direct connection to force our signal to be called, even if our
    // owner object is being destroyed after its thread's event loop has shut
    // down. The slot is thread-safe.
    connect(obj, SIGNAL(destroyed(QObject*)),
            this, SLOT(ownerDestroyed()),
            Qt::DirectConnection);
    }
}

//-----------------------------------------------------------------------------
void qtStatusSourcePrivate::ownerDestroyed()
{
  emit this->ownerDestroyed({this});
}

//-----------------------------------------------------------------------------
qtStatusSource::qtStatusSource(QObject* owner)
  : d_ptr(new qtStatusSourcePrivate(owner))
{
  this->setName(owner);
}

//-----------------------------------------------------------------------------
qtStatusSource::qtStatusSource(const qtStatusSource& other)
  : d_ptr(other.d_ptr)
{
}

//-----------------------------------------------------------------------------
qtStatusSource::qtStatusSource(qtStatusSourcePrivate* d)
  : d_ptr(
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
          d->sharedFromThis())
#else
          QWeakPointer<qtStatusSourcePrivate>(d).toStrongRef())
#endif
{
}

//-----------------------------------------------------------------------------
qtStatusSource::~qtStatusSource()
{
}

//-----------------------------------------------------------------------------
void qtStatusSource::setName(QObject* obj)
{
  QTE_D(qtStatusSource);

  d->displayIdentifier.clear();
  QDebug debug(&d->displayIdentifier);
  debug << obj;
  if (obj == d->owner)
    {
    d->displayIdentifier = d->displayIdentifier.trimmed();
    d->ownerIdentifier = d->displayIdentifier;
    }
  else
    {
    debug << '(' << qPrintable(d->ownerIdentifier) << ')';
    d->displayIdentifier = d->displayIdentifier.trimmed();
    }
}

//-----------------------------------------------------------------------------
qtStatusSource& qtStatusSource::operator=(const qtStatusSource& other)
{
  this->d_ptr = other.d_ptr;
  return *this;
}

//-----------------------------------------------------------------------------
bool qtStatusSource::operator==(const qtStatusSource& other) const
{
  return this->owner() == other.owner();
}

//-----------------------------------------------------------------------------
const QObject* qtStatusSource::owner() const
{
  QTE_D_CONST(qtStatusSource);
  return d->owner;
}

//-----------------------------------------------------------------------------
bool qtStatusSource::isOwnerDestroyed() const
{
  QTE_D_CONST(qtStatusSource);
  return d->ownerRef.isNull();
}

//-----------------------------------------------------------------------------
QDebug& operator<<(QDebug& dbg, const qtStatusSource& ss)
{
  dbg << qPrintable(ss.d_ptr->displayIdentifier);
  return dbg.space();
}
