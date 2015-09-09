/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtThread_h
#define __qtThread_h

/// \file

#include <QThread>

#include "qtGlobal.h"

class qtBareThreadPrivate;

//-----------------------------------------------------------------------------
/// Base class for creating QObject's that live in their own thread.
///
/// Unlike QThread, qtBareThread subclasses are self-hosting; that is, the
/// object 'lives' in the thread it represents. This makes it easy to write
/// worker classes that live in their own threads. Object slots (as long as
/// they are dispatched using Qt::AutoConnection or Qt::QueuedConnection) will
/// be invoked in the object's own thread.
///
/// qtBareThread is not meant to be used directly. Use one of the subclasses
/// instead.
///
/// \sa qtThread, qtThreadedObject, qtInternallyThreadedObject
class QTE_EXPORT qtBareThread
{
public:
  /// Destructor.
  ///
  /// Note that the default destructor does not wait on the thread to end.
  /// Users should ensure that objects derived from qtBareThread are not
  /// destroyed while their thread is still active.
  virtual ~qtBareThread();

  /// Begin execution of the thread.
  virtual void start();

  /// Notify the thread's event loop to exit.
  virtual void quit();

  /// Wait for the thread to exit.
  ///
  /// This method blocks the caller until the thread associated with the owning
  /// instance has completed execution, or the specified timeout (in
  /// milliseconds) has elapsed. If \p time is \c ULONG_MAX (the default), the
  /// call will never time out. If the thread is not running, the call returns
  /// immediately.
  ///
  /// \return \c true if the thread is not running on return, \c false if the
  ///         call timed out.
  virtual bool wait(unsigned long time = ULONG_MAX);

  /// Test if the thread is running.
  virtual bool isRunning() const;

  /// Block execution for specified number of seconds.
  static void sleep(unsigned long seconds);
  /// Block execution for specified number of milliseconds.
  static void msleep(unsigned long milliseconds);
  /// Block execution for specified number of microseconds.
  static void usleep(unsigned long microseconds);

protected:
  QTE_DECLARE_PRIVATE_PTR(qtBareThread)

  explicit qtBareThread(QObject* self);

  /// Starting point for the thread.
  ///
  /// This method executes in the object-thread context after start() is
  /// called. The default implementation executes the thread's event loop,
  /// and can be called explicitly for that purpose. Reimplement it to do other
  /// useful work, or to add startup and shutdown code in the thread's context.
  ///
  /// The thread ends when the reimplemented call to this method returns.
  ///
  /// \sa QThread::run
  virtual void run();

private:
  QTE_DECLARE_PRIVATE(qtBareThread)
  Q_DISABLE_COPY(qtBareThread)
};

//-----------------------------------------------------------------------------
/// Template class to wrap a QObject subclass in a protected thread.
///
/// The qtInternallyThreadedObject template class is used to wrap a QObject
/// subclass in a thread. The thread-related methods will have \c protected
/// access level. See qtBareThread for more details.
///
/// \sa qtBareThread, qtThreadedObject
template <typename T>
class qtInternallyThreadedObject : public T, protected qtBareThread
{
protected:
  /// Constructs the threaded object.
  ///
  /// On return, the QObject represented by \c this is owned by the thread
  /// represented by its qtBareThread.
  qtInternallyThreadedObject() : qtBareThread(this) {}
};

//-----------------------------------------------------------------------------
/// Template class to wrap a QObject subclass in a public thread.
///
/// The qtThreadedObject template class is used to wrap a QObject subclass in a
/// thread. The thread-related methods will have \c public access level. See
/// qtBareThread for more details.
///
/// It is possible to wrap a plain QObject, however it is recommended to use
/// the already-existing qtThread instantiation of this template for that
/// purpose.
///
/// \sa qtBareThread, qtInternallyThreadedObject, qtThread
template <typename T>
class qtThreadedObject : public T, public qtBareThread
{
protected:
  /// Constructs the threaded object.
  ///
  /// On return, the QObject represented by \c this is owned by the thread
  /// represented by its qtBareThread.
  qtThreadedObject() : qtBareThread(this) {}
};

//-----------------------------------------------------------------------------
/// Base class for a QObject wrapped in a public thread.
///
/// qtThread is a convenience typedef for a qtThreadedObject deriving from
/// QObject. It is useful for self-hosting threaded objects that do not need
/// additional functionality from a QObject-derived base class.
///
/// \sa qtThreadedObject, qtBareThread
typedef qtThreadedObject<QObject> qtThread;

#endif
