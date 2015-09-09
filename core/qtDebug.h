/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDebug_h
#define __qtDebug_h

/// \file

#include <QDebug>
#include <QSharedPointer>
#include <QString>

#include "qtDebugArea.h"
#include "qtGlobal.h"

#ifndef __QTE_FUNCTION__
#  if defined DOXYGEN
/// Complete name of the current execution context.
///
/// This macro is a convenience symbol which evaluates to a string literal
/// giving the name of the current execution context (function). Where
/// supported by the compiler, it expands to the full function signature with
/// return and argument types. Otherwise it expands to the undecorated function
/// name.
#    define __QTE_FUNCTION__
#  elif defined Q_CC_MSVC
#    define __QTE_FUNCTION__ __FUNCDNAME__
#  elif defined Q_CC_GNU
#    define __QTE_FUNCTION__ __PRETTY_FUNCTION__
#  else
#    define __QTE_FUNCTION__ __FUNCTION__
#  endif
#endif

#ifndef QTE_EXPORT_DEBUG_AREA
/// Declare an exported qtDebug area.
///
/// This macro is used to declare or define a qtDebug area that is exported by
/// a library. It is the same as #QTE_DEBUG_AREA, except that the ABI
/// decoration symbol for your library may be given as the first parameter
/// (\p abi).
#  define QTE_EXPORT_DEBUG_AREA(abi, prefix, area, default_state) \
    extern abi const qtDebugAreaAccessor& prefix##area
#endif

/// Declare a qtDebug area.
///
/// This macro is used to declare or define a qtDebug area. The area name is
/// the concatenation of \p prefix and \p area. Only \p area will appear in
/// debug output. The default state (enabled or disabled) may be specified by
/// \p default_state. Both \p prefix and \p area must be valid C++ identifiers.
/// \p default_state must be an expression returning a \c bool which can be
/// evaluated in the scope where the debug areas are defined, and is safely
/// executable from an arbitrary thread (using \c true or \c false is
/// recommended).
///
/// \par Example:
/// \code{.cpp}
/// QTE_DEBUG_AREA(qtd, KstReader,     true)
/// QTE_DEBUG_AREA(qtd, StatusManager, false)
/// \endcode
#define QTE_DEBUG_AREA(prefix, area, default_state) \
  QTE_EXPORT_DEBUG_AREA(,prefix, area, default_state)

//-----------------------------------------------------------------------------
/// Area-based debug object.
///
/// The qtDebug class provides area-based debug logging by allowing the user
/// to instantiate an instance from a registered debug area. Areas can be
/// turned on or off by the user via QSettings. The returned qtDebug instance
/// will either display a debug message if the area is enabled, or silently
/// consume input if the area is disabled. Output is prefixed with the name of
/// the area.
///
/// Debug areas are declared in a header file using the #QTE_DEBUG_AREA macro,
/// and have the user-opaque type ::qtDebugAreaAccessor. Areas must have a
/// unique instantiation in a source file, which is usually done by including
/// \c <qtDebugImpl.h> before the header that declares the debug areas. This
/// special header contains a definition of #QTE_DEBUG_AREA that creates
/// definitions of the debug areas rather than declarations. Areas are
/// initialized (and their enabled state set) at first use in a thread-safe
/// manner. After initialization, an area's enabled state cannot be changed.
///
/// Writing output to qtDebug works in the same manner as QDebug; that is,
/// spaces and a newline are added automatically. qtDebug uses QDebug
/// internally, and so it able to output any object type that QDebug can
/// output. Adding support for new object types should be done by overloading
/// \c operator<<(QDebug&, \c T).
///
/// qtDebug employs a clever redirection trick such that code following a
/// qtDebug instantiation will not execute if the debug area is disabled. This
/// also applies if QT_NO_DEBUG_OUTPUT (which disables all debugging output) is
/// defined. As a consequence, you should take care that the evaluation of
/// expressions serialized to qtDebug do not have side effects.
///
/// \sa QDebug, QTE_DEBUG_AREA
class QTE_EXPORT qtDebug
{
public:
  /// Create instance from specified area.
  ///
  /// This constructor creates a qtDebug instance from the specified debug
  /// area. If the area is enabled, the qtDebug instance will write output
  /// via QDebug, prefixed with the area name. Otherwise, the instance will
  /// produce no output. The line will be terminated when all copies of an
  /// instance are destroyed or reassigned.
  qtDebug(qtDebugAreaAccessor);

  /// Copy constructor.
  ///
  /// Create a copy of a qtDebug instance. The instance will write to the same
  /// output line as the original instance. The line will be terminated when
  /// all copies of an instance are destroyed or reassigned.
  qtDebug(const qtDebug& other) : d(other.d) {}

  ~qtDebug() {}

  /// Assignment operator.
  ///
  /// Assign another qtDebug instance to this instance. This instance will
  /// begin writing to the same output line as the other instance. The line
  /// will be terminated when all copies of an instance are destroyed or
  /// reassigned.
  qtDebug& operator=(const qtDebug& other)
    { this->d = other.d; return *this; }

#if !defined(QT_NO_DEBUG_OUTPUT) || defined(DOXYGEN)
  /// Write object to the debug stream.
  ///
  /// This operator writes a string representation of the specified object to
  /// the debug stream, if the instance's area is enabled; otherwise it does
  /// nothing. The object is converted to a string using
  /// \c operator<<(QDebug&, \c T).
  template <typename T> inline qtDebug& operator<<(const T& t)
    { if (this->d) { (*this->d) << t; } return *this; }
#else
  template <typename T> inline qtDebug& operator<<(const T&)
    { return *this; }
#endif

  /// Test if object is valid.
  ///
  /// This method tests if a qtDebug instance is valid (i.e. will produce
  /// output). Generally, an instance is valid if it was constructed from an
  /// active debug area, or from another valid instance.
  bool isValid() const { return this->d; }

  /// Test if area is active.
  ///
  /// This method tests if the specified area is active. This may be useful to
  /// if debugging requires performing steps that are complex and/or time
  /// consuming that cannot be done inline, to avoid such overhead if the
  /// debugging area is disabled.
  static bool isAreaActive(qtDebugAreaAccessor);

  /// Static instance of an invalid area.
  ///
  /// This variable declares a debug area which is invalid (always disabled).
  static const qtDebugAreaAccessor InvalidArea;

protected:
  QSharedPointer<QDebug> d;
};

#ifdef QT_NO_DEBUG_OUTPUT

#  define qtDebug_DEBUG_MACRO while(false) qtDebug

#else

#  define qtDebug_DEBUG_MACRO(area) \
  for (qtDebugHelper _dbg(area); _dbg; _dbg.finish()) _dbg.debug()

#  ifndef DOXYGEN

//-----------------------------------------------------------------------------
class qtDebugHelper
{
public:
  inline explicit qtDebugHelper(qtDebugAreaAccessor area) :
    Debug(area), Active(this->Debug.isValid()) {}
  inline operator bool() const { return this->Active; }
  inline void finish() { this->Active = false; }
  inline qtDebug& debug() { return this->Debug; }

protected:
  qtDebug Debug;
  bool Active;
};

#  endif

#endif

#define qtDebug(area) qtDebug_DEBUG_MACRO(area)

#endif
