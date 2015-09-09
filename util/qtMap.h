/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtMap_h
#define __qtMap_h

#include <QtGlobal>

#include "qtGlobal.h"

namespace qtUtil
{

#if defined(DOXYGEN)

// Human-readable template declarations. The declarations here are intended to
// provide a conceptual view of the templates provided by this header, whereas
// the actual definitions are more finely specified (i.e. each declaration in
// this section corresponds to two or more definitions as seen by the compiler,
// for various container flavors).
//
// For similar reasons, these declarations, rather than the compiler-visible
// definitions, are used to generate the documentation.

/// Execute a function for each item in a container.
///
/// This function iterates over each item in \p list, and executes \p func for
/// each item, where \p func takes as its first parameter an argument of type
/// equal to the item type. Additional parameters may be specified, and are
/// passed directly to \p func for each item. The \p list may be any of QList,
/// QHash, QMap or QSet. For keyed containers (QHash, QMap), only the value is
/// passed to \p func.
///
/// \sa qtUtil::mapWithKey
template <typename LT, typename MF>
void map(const QtGenericContainer<LT>& list, MF func, ...);

/// Execute a function for each item in a keyed container.
///
/// This function iterates over each item in \p list, and executes \p func for
/// each item, where \p func takes as its first parameter an argument of type
/// equal to the container's key type, and as its second parameter an argument
/// of type equal to the container's value type. Additional parameters may be
/// specified, and are passed directly to \p func for each item. The \p list
/// may be either a QHash or QMap.
///
/// \sa qtUtil::map
template <typename KT, typename VT, typename MF>
void mapWithKey(QtKeyedContainer<KT, VT> const& list, MF func, ...);

/// Execute a bound method for each item in a container.
///
/// This function is equivalent to qtUtil::map, except that \p func is a
/// pointer to a bound member function, which will be called against the object
/// instance \p mapObject. The object instance may be passed by reference, or
/// as a pointer.
///
/// \sa qtUtil::mapBoundWithKey
template <typename LT, typename MO, typename MF>
void mapBound(const QtGenericContainer<LT>& list, MO mapObject,
              MF func, ...);

/// Execute a bound method for each item in a keyed container.
///
/// This function is equivalent to qtUtil::mapWithKey, except that \p func is a
/// pointer to a bound member function, which will be called against the object
/// instance \p mapObject. The object instance may be passed by reference, or
/// as a pointer.
///
/// \sa qtUtil::mapBound
template <typename KT, typename VT, typename MO, typename MF>
void mapBoundWithKey(QtKeyedContainer<KT, VT> const& list, MO mapObject,
                     MF func, ...);

#elif __cplusplus >= 201103L

// C++11 variadac-template declarations.

#define QTMAP_EXTRA_TEMPLATE , typename... Args
#define QTMAP_EXTRA_ARGS , const Args&... args
#define QTMAP_EXTRA_INVOKE , args...
#include "qtMapImpl.h"
#undef QTMAP_EXTRA_TEMPLATE
#undef QTMAP_EXTRA_ARGS
#undef QTMAP_EXTRA_INVOKE

#else

// Pre-C++11 template definitions. Since we don't have variadic templates, we
// 'fake it' by declaring several versions, taking zero to (currently) two
// additional arguments. Support for more arguments can be added easily, by
// replicating later invocations of the "implementation" header inclusion.
// Repeated inclusion of the implementation header is used to expand each set
// of variations.

#define QTMAP_EXTRA_TEMPLATE
#define QTMAP_EXTRA_ARGS
#define QTMAP_EXTRA_INVOKE
#include "qtMapImpl.h"
#undef QTMAP_EXTRA_TEMPLATE
#undef QTMAP_EXTRA_ARGS
#undef QTMAP_EXTRA_INVOKE

#define QTMAP_EXTRA_TEMPLATE , typename X1
#define QTMAP_EXTRA_ARGS , const X1& x1
#define QTMAP_EXTRA_INVOKE , x1
#include "qtMapImpl.h"
#undef QTMAP_EXTRA_TEMPLATE
#undef QTMAP_EXTRA_ARGS
#undef QTMAP_EXTRA_INVOKE

#define QTMAP_EXTRA_TEMPLATE , typename X1, typename X2
#define QTMAP_EXTRA_ARGS , const X1& x1, const X2& x2
#define QTMAP_EXTRA_INVOKE , x1, x2
#include "qtMapImpl.h"
#undef QTMAP_EXTRA_TEMPLATE
#undef QTMAP_EXTRA_ARGS
#undef QTMAP_EXTRA_INVOKE

#endif

}

#endif
