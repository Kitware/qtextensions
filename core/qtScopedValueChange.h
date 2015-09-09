/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtScopedValueChange_h
#define __qtScopedValueChange_h

#include <QObject>
#include <QString>
#include <QVariant>

#include "qtGlobal.h"

class QTreeWidget;
class QObject;

// Generic class template
template <typename ObjectType, typename Helper, typename ValueType>
class qtGenericScopedValueChange
{
public:
  qtGenericScopedValueChange(ObjectType* o, ValueType value)
    : object(o), oldValue(Helper::get(o))
    { Helper::set(o, value); }

  ~qtGenericScopedValueChange()
    { Helper::set(this->object, this->oldValue); }

protected:
  ObjectType* object;
  ValueType oldValue;

private:
  Q_DISABLE_COPY(qtGenericScopedValueChange)
};

// Generic class template acting on a POD reference
template <typename ValueType>
class qtScopedValueChange
{
public:
  qtScopedValueChange(ValueType& valueRef, ValueType newValue)
    : vref(valueRef), oldValue(valueRef)
    { valueRef = newValue; }

  ~qtScopedValueChange()
    { this->vref = this->oldValue; }

protected:
  ValueType& vref;
  ValueType oldValue;

private:
  Q_DISABLE_COPY(qtScopedValueChange)
};

// Generic class template with fixed value
template <typename ObjectType, typename Helper,
          typename ValueType, ValueType Value>
class qtGenericScopedValueFixedChange
  : public qtGenericScopedValueChange<ObjectType, Helper, ValueType>
{
public:
  qtGenericScopedValueFixedChange(ObjectType* o)
    : qtGenericScopedValueChange<ObjectType, Helper, ValueType>(o, Value) {}

  ~qtGenericScopedValueFixedChange() {}

private:
  Q_DISABLE_COPY(qtGenericScopedValueFixedChange)
};

// Generic class for QObject properties
class QTE_EXPORT qtScopedPropertyChange
{
public:
  qtScopedPropertyChange(QObject* o, const QString& name, QVariant value)
    : object(o), propertyName(name)
    {
    oldValue = object->property(qPrintable(name));
    object->setProperty(qPrintable(name), value);
    }

  ~qtScopedPropertyChange()
    {
    this->object->setProperty(qPrintable(this->propertyName),
                              this->oldValue);
    }

protected:
  QObject* object;
  QString propertyName;
  QVariant oldValue;

private:
  Q_DISABLE_COPY(qtScopedPropertyChange)
};

// Generic class to block QObject signals
class QTE_EXPORT qtScopedBlockSignals
{
public:
  qtScopedBlockSignals(QObject* o) : object(o)
    { oldValue = o->blockSignals(true); }

  ~qtScopedBlockSignals()
    { this->object->blockSignals(this->oldValue); }

protected:
  QObject* object;
  bool oldValue;

private:
  Q_DISABLE_COPY(qtScopedBlockSignals)
};

// Helper for QWidget updating
class QTE_EXPORT qtSvcWidgetUpdateHelper
{
public:
  static bool get(QWidget*);
  static void set(QWidget*, bool);
};

// Helper for QTreeWidget sorting
class QTE_EXPORT qtSvcTreeSortHelper
{
public:
  static bool get(QTreeWidget*);
  static void set(QTreeWidget*, bool);
};

// Specialization typedefs
typedef qtGenericScopedValueFixedChange<
          QWidget, qtSvcWidgetUpdateHelper, bool, false>
        qtScopedBlockUpdates;
typedef qtGenericScopedValueFixedChange<
          QTreeWidget, qtSvcTreeSortHelper, bool, false>
        qtDelayTreeSorting;

#endif
