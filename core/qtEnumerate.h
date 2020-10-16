// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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

//-----------------------------------------------------------------------------
template <typename Item> class qtChildEnumerator
{
public:
    class iterator;

    qtChildEnumerator(Item* parent) : p{parent} {}

    iterator begin() const { return {p, 0}; }
    iterator end() const { return {p, p->childCount()}; }

protected:
    Item* const p;
};

//-----------------------------------------------------------------------------
template <typename Item> class qtChildEnumerator<Item>::iterator
{
public:
    using ChildItem = decltype(std::declval<Item>().child(0));
    using IndexType = decltype(std::declval<Item>().childCount());

    ChildItem operator*() const { return p->child(i); }
    iterator& operator++() { ++i; return *this; }

    bool operator==(iterator const& other) const
    { return p == other.p && i == other.i; }

    bool operator!=(iterator const& other) const
    { return p != other.p || i != other.i; }

protected:
    friend class qtChildEnumerator<Item>;
    iterator(Item* parent, IndexType index) : p{parent}, i{index} {}

    Item* const p;
    IndexType i;
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
/// for (auto const iter : qtEnumerate(map))
///     qDebug() << "key" << iter.key() << "value" << iter.value();
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

//-----------------------------------------------------------------------------
/// Construct an enumerating adapter over a parent item.
///
/// This function constructs an enumerable adapter (i.e. one that is compatible
/// with #foreach or range-based \c for loop) over an object which has children
/// (that is, which implements the methods \c child and \c childCount). This
/// is intended to be used with e.g. QTreeWidgetItem.
///
/// \par Example:
/// \code{.cpp}
/// typedef QHash<int, QString> MyMap;
/// MyMap map = createMap();
/// for (auto* const child, qtChildren(tree->invisibleRootItem()))
///     qDebug() << child->text(0);
/// \endcode
template <typename Item>
qtChildEnumerator<Item> qtChildren(Item* parent)
{
    return {parent};
}

#if !defined(DOXYGEN) && !(__GNUC__ == 4 && __GNUC_MINOR__ < 9)

//-----------------------------------------------------------------------------
// Delete dangerous specializations
//
// These explicitly deleted overloads exists to prevent users from calling
// qtEnumerate[Mutable] on a temporary container, which would lead to a
// dangling reference. Unfortunately, GCC 4.8 has a bug in overload resolution
// related to r-value references, which causes use of qtEnumerate[Mutable] to
// be ambiguous if these overloads are present. Since they exists only to
// prevent users from using the function in a broken manner, they aren't
// necessary for correct use, so just omit them when compiling with GCC 4.8.
// Correct code will still work; users of GCC 4.8 will just lose out on
// incorrect code being caught as an error.
template <typename T> void qtEnumerate(T&&) = delete;
template <typename T> void qtEnumerateMutable(T&&) = delete;

#endif

#endif
