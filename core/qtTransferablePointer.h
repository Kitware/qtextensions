/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtTransferablePointer_h
#define __qtTransferablePointer_h

#include <QScopedPointer>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4521) // multiple copy ctors specified
#endif

//-----------------------------------------------------------------------------
/// Class to hold a "transferable", automatically deleted pointer to a
/// dynamically allocated object.
///
/// The qtTransferablePointer class stores a pointer to a dynamically
/// allocated object, and deletes it when the reference goes out of scope. It
/// is similar to (and in fact, a subclass of) QScopedPointer, but adds a
/// "transferable" property. Making a copy of a mutable qtTransferablePointer
/// "transfers" ownership of the object to the new qtTransferablePointer
/// instance. This property allows dynamically allocated objects to be safely
/// passed to a single receiver across threads, without the overhead of using
/// an atomically reference-counted pointer like QSharedPointer. Using a
/// qtTransferablePointer ensures that the object will still be deleted if the
/// receiving method never executes (e.g. because its event loop is no longer
/// executing).
///
/// When declaring a qtTransferablePointer, an optional \p Cleanup functor
/// may be specified. This must be a class or struct having a method
/// \c cleanup(\c T*) which is invoked to destroy the object. The default
/// functor simply invokes <code>operator delete</code> on the object.
///
/// \note In order to support transference via Qt queued method invocation
///       (i.e. QMetaObject::invokeMethod), qtTransferablePointer allows Qt's
///       metatype system to access its standard copy constructor, which
///       violates const-correctness by modifying the source instance. The
///       public "transfer" constructor correctly takes a mutable reference to
///       the source instance.
///
/// \par Thread Safety:
/// qtTransferablePointer is reentrant, but not generally thread safe. Using
/// qtTransferablePointer to pass an object pointer to another thread is safe,
/// provided that the underlying object is not accessed by the original thread
/// once it has been handed off, and that the qtTransferablePointer seen by the
/// receiving thread is not accessed in any way (again, by the original thread)
/// once it becomes visible to the receiving thread.
template <typename T, typename Cleanup = QScopedPointerDeleter<T> >
class qtTransferablePointer : public QScopedPointer<T, Cleanup>
{
public:
  /// Default constructor.
  ///
  /// Create a new qtTransferablePointer instance, owning the specified object.
  /// If \p data is null, the instance will be invalid.
  explicit inline qtTransferablePointer(T* data = 0)
    : QScopedPointer<T, Cleanup>(data) {}

  /// Destroy this instance.
  ///
  /// If the instance owns an object, it is destroyed, using the cleanup
  /// handler specified when the instance was declared.
  inline ~qtTransferablePointer() {}

  /// Transfer constructor.
  ///
  /// Create a new qtTransferablePointer instance from an existing instance.
  /// The new instance takes ownership of the other instance's object. On
  /// completion, \p other becomes invalid.
  inline qtTransferablePointer(qtTransferablePointer<T, Cleanup>& other)
    : QScopedPointer<T, Cleanup>(0)
    {
    this->swap(other);
    }

protected:
  template <typename FT> friend void* qMetaTypeConstructHelper(const FT*);

  inline qtTransferablePointer(qtTransferablePointer<T, Cleanup> const& o)
    : QScopedPointer<T, Cleanup>(0)
    {
    qtTransferablePointer<T, Cleanup>& other =
      const_cast<qtTransferablePointer<T, Cleanup>&>(o);
    this->swap(other);
    }

private:
  qtTransferablePointer& operator=(const qtTransferablePointer&);
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
