/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtGet_h
#define __qtGet_h

//-----------------------------------------------------------------------------
/// Return an item from an associative container, or \c nullptr.
///
/// This helper function returns the pointer to an item in an associative
/// container, if an item with the requested key exists, or \c nullptr
/// otherwise.
///
/// \par Example:
/// \code{.cpp}
/// // QHash<int, ComplexType> container;
/// if (auto* const item = qtGet(container, key))
///   ...do stuff...
/// else
///   ..handle missing item...
/// \endcode
template <typename Container, typename Key>
auto qtGet(Container const& c, Key const& key)
    -> decltype(std::addressof(*c.find(key)))
{
  auto const iter = c.find(key);
  return (iter == c.end() ? nullptr : std::addressof(*iter));
}

//-----------------------------------------------------------------------------
/// \copydoc qtGet(Container const& c, Key const& key)
template <typename Container, typename Key>
auto qtGet(Container& c, Key const& key)
    -> decltype(std::addressof(*c.find(key)))
{
  auto const iter = c.find(key);
  return (iter == c.end() ? nullptr : std::addressof(*iter));
}

// HACK: GCC 4.8 does not properly disambiguate the rvalue overload, so we just
// disable it.
#ifdef __GNUC__
#  if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)
#    define QTE_DECLARE_RVALUE_OVERLOAD 1
#  else
#    define QTE_DECLARE_RVALUE_OVERLOAD 0
#  endif
#else
#  define QTE_DECLARE_RVALUE_OVERLOAD 1
#endif

//-----------------------------------------------------------------------------
#if QTE_DECLARE_RVALUE_OVERLOAD
template <typename Container, typename Key>
void qtGet(Container&& c, Key const& key) = delete;
#else
#  warning "Not declaring rvalue overload of qtGet() because you are using a broken version of GCC (4.8 or earlier)."
#endif
#undef QTE_DECLARE_RVALUE_OVERLOAD

#endif
