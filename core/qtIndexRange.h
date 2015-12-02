/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

// Based on code written by Matthew Woehlke and used by permission.

#ifndef __qtIndexRange_h
#define __qtIndexRange_h

//-----------------------------------------------------------------------------
template <typename T> class qtIndexRangeType
{
public:
    class iterator;

    qtIndexRangeType(T count) : End{count} {}

    iterator begin() const { return {T{0}}; }
    iterator end() const { return {End}; }

protected:
    T End;
};

//-----------------------------------------------------------------------------
template <typename T> class qtIndexRangeType<T>::iterator
{
public:
    T operator*() const { return Value; }
    iterator& operator++() { ++Value; return *this; }

    bool operator==(iterator const& other) const
    { return Value == other.Value; }

    bool operator!=(iterator const& other) const
    { return Value != other.Value; }

protected:
    friend class qtIndexRangeType<T>;
    iterator(T value) : Value{value} {}

    T Value;
};

//-----------------------------------------------------------------------------
template <typename T> auto qtIndexRange(T count) -> qtIndexRangeType<T>
{
    return {count};
}

#endif
