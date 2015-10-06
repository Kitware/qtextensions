/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtUiStateItem_h
#define __qtUiStateItem_h

#include "qtUiState.h"

#include <QVariant>
#include <QWeakPointer>

//-----------------------------------------------------------------------------
/// Template utility class to save and restore UI state of an object.
///
/// qtUiState::Item provides a convenience implementation of a
/// qtUiState::AbstractItem that takes an object pointer and pointers to get
/// and set methods, which can be used to conveniently persist a property of
/// many object types (e.g. most properties of most Qt widgets). This class is
/// used extensively by qtUiState itself, and is provided as a convenience for
/// users that wish to add persistence to custom widgets, or to attributes not
/// supported by qtUiState directly.
///
/// \par Example:
/// \code{.cpp}
/// auto const item = new qtUiState::Item<QString, QLabel>(
///   label, &QLabel::
/// myUiState.map("LabelText", item);
/// \endcode
template <typename T, typename O>
class qtUiState::Item : public qtUiState::AbstractItem
{
public:
    typedef T(O::*ReadMethod)() const;
    typedef void (O::*WriteRefMethod)(T const&);
    typedef void (O::*WriteValMethod)(T);

    /// Construct item from object pointer and read/write method pointers.
    ///
    /// \param object Pointer to the object on which this Item operates.
    /// \param read Pointer to a member function which will return the value of
    ///             the object property to be persisted.
    /// \param write Pointer to a member function which will set the value of
    ///              the object property to be persisted.
    Item(O* object, ReadMethod read, WriteRefMethod write);

    /// \copydoc Item(O*, ReadMethod, WriteRefMethod)
    Item(O* object, ReadMethod read, WriteValMethod write);

    /// \copydoc qtUiState::AbstractItem::value
    ///
    /// Internally, this calls the read method that was provided at
    /// construction on the object that was provided at construction.
    virtual QVariant value() const QTE_OVERRIDE;

    /// \copydoc qtUiState::AbstractItem::setValue
    ///
    /// Internally, this calls the write method that was provided at
    /// construction on the object that was provided at construction.
    virtual void setValue(QVariant const&) QTE_OVERRIDE;

protected:
    QWeakPointer<O> object;
    ReadMethod const read;
    WriteRefMethod const writeRef;
    WriteValMethod const writeVal;
};

//-----------------------------------------------------------------------------
template <typename T, typename O>
qtUiState::Item<T, O>::Item(
    O* object, ReadMethod read, WriteRefMethod write)
    : object(object), read(read), writeRef(write), writeVal(0)
{
}

//-----------------------------------------------------------------------------
template <typename T, typename O>
qtUiState::Item<T, O>::Item(
    O* object, ReadMethod read, WriteValMethod write)
    : object(object), read(read), writeRef(0), writeVal(write)
{
}

//-----------------------------------------------------------------------------
template <typename T, typename O>
QVariant qtUiState::Item<T, O>::value() const
{
    if (!this->object)
        return QVariant();

    return (*(object.data()).*this->read)();
}

//-----------------------------------------------------------------------------
template <typename T, typename O>
void qtUiState::Item<T, O>::setValue(QVariant const& data)
{
    if (!this->object)
        return;

    if (this->writeVal)
        (*(object.data()).*this->writeVal)(data.value<T>());
    else
        (*(object.data()).*this->writeRef)(data.value<T>());
}

#endif
