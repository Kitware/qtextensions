/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtEnumerate_h
#define __qtEnumerate_h

#include <iterator>
#include <type_traits>

#ifndef DOXYGEN

//-----------------------------------------------------------------------------
namespace qtRangeDetail {
    using std::begin;
    using std::end;

    template <typename Range>
    struct qtRangeHelper
    {
        static auto beginHelper(Range const& range)
            -> decltype(begin(range))
        { return begin(range); }

        static auto endHelper(Range const& range)
            -> decltype(end(range))
        { return end(range); }

        using Iterator = decltype(beginHelper(std::declval<Range>()));
    };
}

//-----------------------------------------------------------------------------
template <typename Range> class qtEnumerator
{
public:
    class iterator;

    qtEnumerator(Range& range) : r(range) {}

    iterator begin() const { return {d::beginHelper(r)}; }
    iterator end() const { return {d::endHelper(r)}; }

protected:
    using d = typename qtRangeDetail::qtRangeHelper<Range>;

    Range& r;
};

//-----------------------------------------------------------------------------
template <typename Range> class qtEnumerator<Range>::iterator
{
public:
    using Iterator = typename qtRangeDetail::qtRangeHelper<Range>::Iterator;

    Iterator operator*() const { return i; }
    iterator& operator++() { ++i; return *this; }

    bool operator==(iterator const& other) const
    { return i == other.i; }

    bool operator!=(iterator const& other) const
    { return i != other.i; }

protected:
    friend class qtEnumerator<Range>;
    iterator(Iterator const& iter) : i{iter} {}

    Iterator i;
};

#endif

//-----------------------------------------------------------------------------
/// Construct an enumerating adapter over a range.
///
/// This function constructs an enumerable adapter (i.e. one that is compatible
/// with #foreach or range-based \c for loop) over a range which yields the
/// original, rather than dereferenced, iterator as the enumerated item. This
/// is especially useful for iterating over Qt associative containers when the
/// key is required, as the dereferenced iterator would provide only the value.
///
/// While intended for Qt associative containers, this adapter should be usable
/// on any enumerable container to retrieve the iterator as the item type in
/// a #foreach or range-based \c for loop.
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
///
/// \sa qtEnumerateMutable
template <typename Range>
qtEnumerator<Range const> qtEnumerate(Range& range)
{
    return {range};
}

//-----------------------------------------------------------------------------
/// Construct an enumerating adapter over a range.
///
/// This function is equivalent to #qtEnumerate, except that it yields mutable
/// iterators (which allow modification of the underlying item) rather than
/// immutable iterators.
///
/// \sa qtEnumerate
template <typename Range>
qtEnumerator<Range> qtEnumerateMutable(Range& range)
{
    return {range};
}

#ifndef DOXYGEN

// Delete dangerous specializations
template <typename T> void qtEnumerate(T&&) = delete;
template <typename T> void qtEnumerateMutable(T&&) = delete;

#endif

#endif
