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
template <typename Container, typename Key>
void qtGet(Container&& c, Key const& key) = delete;

#endif
