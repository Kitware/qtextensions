/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtEnumerate_h
#define __qtEnumerate_h

#ifndef DOXYGEN

//-----------------------------------------------------------------------------
template <typename Container> class qtEnumerator
{
public:
    class iterator;

    qtEnumerator(Container const& container) : c(container) {}

    iterator begin() const { return {c.begin()}; }
    iterator end() const { return {c.end()}; }

protected:
    Container const& c;
};

//-----------------------------------------------------------------------------
template <typename Container> class qtEnumerator<Container>::iterator
{
public:
    typedef typename Container::const_iterator Iterator;

    Iterator operator*() const { return i; }
    iterator& operator++() { ++i; return *this; }

    bool operator==(iterator const& other) const
    { return i == other.i; }

    bool operator!=(iterator const& other) const
    { return i != other.i; }

protected:
    friend class qtEnumerator<Container>;
    iterator(Iterator const& iter) : i{iter} {}

    Iterator i;
};

#endif

//-----------------------------------------------------------------------------
/// Construct an enumerating adapter over a Qt associative container.
///
/// This function constructs an enumerable adapter (i.e. one that is compatible
/// with <code>for #each</code> over a Qt associative container which returns
/// the original, rather than dereferenced, iterator as the enumerated item.
/// This allows iterating over an associative container when both the key and
/// the value are required while avoiding the inconvenience and overhead of
/// iterating over the key set.
///
/// While intended for Qt associative containers, this adapter should be usable
/// on any enumerable container to retrieve the iterator as the item type in
/// a <code>for #each</code> loop.
///
/// \note
///   The enumeration adapter does not permit modification of the items in the
///   container.
///
/// \par Example:
/// \code{.cpp}
/// typedef QHash<int, QString> MyMap;
/// MyMap map = createMap();
/// foreach(auto const iter, qtEnumerate(map))
///   qDebug() << "key" << iter.key() << "value" << iter.value();
/// \endcode
template <typename Container>
qtEnumerator<Container> qtEnumerate(Container const& container)
{
    return {container};
}

#endif
