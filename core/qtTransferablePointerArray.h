/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtTransferablePointerArray_h
#define __qtTransferablePointerArray_h

#include "qtGlobal.h"

#include <QScopedPointer>
#include <QtAlgorithms>

template <typename T> class QList;

//-----------------------------------------------------------------------------
/// \cond implementation_details
template <typename T, typename ItemCleanup>
struct qtTransferablePointerArrayDeleter
{
  static inline void cleanup(QList<T*>* list)
    {
    if (list)
      {
      foreach (auto const item, *list)
        ItemCleanup::cleanup(item);
      delete list;
      }
    }
};
/// \endcond

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4521) // multiple copy ctors specified
#endif

//-----------------------------------------------------------------------------
/// Class to hold a "transferable" list of automatically deleted pointers to
/// dynamically allocated objects.
///
/// The qtTransferablePointerArray class stores a dynamic list of pointers to
/// dynamically allocated objects, and deletes them when the reference goes out
/// of scope. It is a conceptual extension of qtTransferablePointer to a list
/// of pointers rather than a single pointer.
///
/// qtTransferablePointerArray is similar to a list of QScopedPointer's, with
/// the addition of a "transferable" property. Making a copy of a mutable
/// qtTransferablePointerArray "transfers" ownership of the objects to the new
/// qtTransferablePointerArray instance. This property allows dynamically
/// allocated objects to be safely passed to a single receiver across threads,
/// without the overhead of using an atomically reference-counted pointer like
/// QSharedPointer. Using a qtTransferablePointerArray ensures that the objects
/// will still be deleted if the receiving method never executes (e.g. because
/// its event loop is no longer executing).
///
/// When declaring a qtTransferablePointerArray, an optional \p Cleanup functor
/// may be specified. This must be a class or struct having a method
/// \c cleanup(\c T*) which is invoked to destroy each object. The default
/// functor simply invokes <code>operator delete</code> on each object.
///
/// \note In order to support transference via Qt queued method invocation
///       (i.e. QMetaObject::invokeMethod), qtTransferablePointerArray allows
///       Qt's metatype system to access its standard copy constructor, which
///       violates const-correctness by modifying the source instance. The
///       public "transfer" constructor correctly takes a mutable reference to
///       the source instance.
///
/// \par Thread Safety:
/// qtTransferablePointerArray is reentrant, but not generally thread safe.
/// Using qtTransferablePointerArray to pass a list of object pointers to
/// another thread is safe, provided that the underlying object is not accessed
/// by the original thread once it has been handed off, and that the
/// qtTransferablePointerArray seen by the receiving thread is not accessed in
/// any way (again, by the original thread) once it becomes visible to the
/// receiving thread.
template <typename T, typename Cleanup = QScopedPointerDeleter<T>>
class qtTransferablePointerArray
  : public QScopedPointer <QList<T*>,
                           qtTransferablePointerArrayDeleter<T, Cleanup>>
{
protected:
  typedef qtTransferablePointerArrayDeleter<T, Cleanup> Deleter;
  typedef QScopedPointer<QList<T*>, Deleter> BaseClass;
  typedef qtTransferablePointerArray<T, Cleanup> ThisClass;

public:
  /// Construct an empty instance.
  ///
  /// This default constructor creates a new qtTransferablePointerArray
  /// intance that initially has no data.
  explicit inline qtTransferablePointerArray()
    : BaseClass(0) {}

  /// Construct an instance from an initial object list.
  ///
  /// This constructor creates a new qtTransferablePointerArray instance,
  /// setting its initial data to \p data. The instance takes ownership of the
  /// objects referenced by \p data, after which \p data is cleared.
  ///
  /// \sa reset()
  explicit inline qtTransferablePointerArray(QList<T*>& data)
    : BaseClass(0)
    {
    this->reset(data);
    }

  /// Destroy this instance.
  ///
  /// Objects owned by the qtTransferablePointerArray instance are destroyed,
  /// using the cleanup handler specified when the instance was declared.
  inline ~qtTransferablePointerArray() {}

  /// Transfer constructor.
  ///
  /// Create a new qtTransferablePointerArray instance from an existing
  /// instance. The new instance takes ownership of the other instance's
  /// objects. On completion, \p other becomes empty.
  inline qtTransferablePointerArray(ThisClass& other)
    : BaseClass(0)
    {
    this->swap(other);
    }

  /// Set this qtTransferablePointerArray instance's object list.
  ///
  /// The qtTransferablePointerArray takes ownership of the objects referenced
  /// by \p data, after which \p data is cleared. Any objects previously owned
  /// by this instance are deleted, using the cleanup handler specified when
  /// the instance was declared.
  void reset(QList<T*>& data)
    {
    this->BaseClass::reset(new QList<T*>(data));
    data.clear();
    }

  /// Test if the object list is empty.
  ///
  /// \return \c true if the array is empty (contains no objects), \c false
  ///         otherwise.
  inline bool isEmpty() const
    {
    return (this->isNull() || this->data()->isEmpty());
    }

  /// Get number of objects in the object list.
  ///
  /// This method returns the number of objects held by the instance.
  inline int count() const
    {
    return (this->isNull() ? 0 : this->data()->count());
    }

  /// Append \p item to object list.
  ///
  /// This method appends \p item to this instance's object list. The
  /// qtTransferablePointerArray instance takes ownership of \p item.
  void append(T* item)
    {
    if (this->isNull())
      {
      this->BaseClass::reset(new QList<T*>);
      }
    this->data()->append(item);
    }

  /// Take object list.
  ///
  /// This method returns the list of objects owned by this
  /// qtTransferablePointerArray instance, transferring ownership of the same
  /// to the caller. On return, the qtTransferablePointerArray instance will be
  /// empty. The caller is responsible for destroying the objects.
  QList<T*> take()
    {
    if (this->isNull())
      {
      return QList<T*>();
      }

    QScopedPointer<QList<T*>> p(this->BaseClass::take());
    return *p.data();
    }

protected:
  template <typename FT> friend void* qMetaTypeConstructHelper(const FT*);

  inline qtTransferablePointerArray(const ThisClass& o)
    : BaseClass(0)
    {
    ThisClass& other = const_cast<ThisClass&>(o);
    this->swap(other);
    }

private:
  qtTransferablePointerArray& operator=(const qtTransferablePointerArray&);
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
