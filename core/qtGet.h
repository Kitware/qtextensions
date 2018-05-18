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

//-----------------------------------------------------------------------------
#if !(__GNUC__ == 4 && __GNUC_MINOR__ < 9)
// This explicitly deleted overload exists to prevent users from calling qtGet
// on a temporary value, which would lead to a dangling reference.
// Unfortunately, GCC 4.8 has a bug in overload resolution related to r-value
// references, which causes use of qtGet to be ambiguous if this overload is
// present. Since it exists only to prevent users from using the function in a
// broken manner, it isn't necessary for correct use, so just omit it when
// compiling with GCC 4.8. Correct code will still work; users of GCC 4.8 will
// just lose out on incorrect code being caught as an error.
template <typename Container, typename Key>
void qtGet(Container&& c, Key const& key) = delete;
#endif

#endif
