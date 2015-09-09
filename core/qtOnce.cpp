/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtOnce.h"

#ifdef Q_OS_WIN

#include <windows.h>

//-----------------------------------------------------------------------------
class qtOnceLock
{
public:
  static qtOnceLock* enqueue(qtOnceGuard& container);
  static void dequeue(qtOnceLock* lock, qtOnceGuard& container);

protected:
  qtOnceLock();
  ~qtOnceLock();

  CRITICAL_SECTION cs;
};

//-----------------------------------------------------------------------------
qtOnceLock::qtOnceLock()
{
  InitializeCriticalSection(&this->cs);
  EnterCriticalSection(&this->cs);
}

//-----------------------------------------------------------------------------
qtOnceLock::~qtOnceLock()
{
  DeleteCriticalSection(&this->cs);
}

//-----------------------------------------------------------------------------
qtOnceLock* qtOnceLock::enqueue(qtOnceGuard& container)
{
  qtOnceLock* lock = new qtOnceLock;
  qtOnceLock* prev = static_cast<qtOnceLock*>(
                       InterlockedExchangePointer(&container.mutex, lock));

  // If there was a previous caller...
  if (prev)
    {
    // ...wait for the previous caller to dequeue...
    EnterCriticalSection(&prev->cs);
    LeaveCriticalSection(&prev->cs);
    // ..then destroy the previous caller
    delete prev;
    }

  return lock;
}

//-----------------------------------------------------------------------------
void qtOnceLock::dequeue(qtOnceLock* lock, qtOnceGuard& container)
{
  // Try to remove self from the queue
  qtOnceLock* last = static_cast<qtOnceLock*>(
                       InterlockedCompareExchangePointer(&container.mutex, 0,
                                                         lock));

  // Release critical section, allowing next thread in the queue to proceed
  LeaveCriticalSection(&lock->cs);

  if (last == lock)
    {
    // If the queue was pointing to us, the queue is now empty and it is safe
    // to delete our lock (otherwise the next lock, with a pointer to our lock,
    // is already on the queue, and will delete our lock when it wakes)
    delete lock;
    }
}

//-----------------------------------------------------------------------------
void qtOnce(qtOnceGuard& guard, void(*func)(void))
{
  if (guard.init)
    {
    // Function has already been called; nothing to do
    return;
    }

  // Insert into wait queue
  qtOnceLock* lock = qtOnceLock::enqueue(guard);

  // Check if we are the first caller to lock the mutex
  if (!guard.init)
    {
    // Yes; go ahead and execute the function...
    (*func)();
    // ...and set the initialization flag so no one else will try to execute
    // it a second time
    InterlockedIncrement(&guard.init);
    }

  // Remove from wait queue
  qtOnceLock::dequeue(lock, guard);
}

#else

//-----------------------------------------------------------------------------
void qtOnce(qtOnceGuard& guard, void(*func)(void))
{
  pthread_once(&guard, func);
}

#endif
