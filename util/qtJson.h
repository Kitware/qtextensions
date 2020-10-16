// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtJson_h
#define __qtJson_h

/// \file

#include <QMap>
#include <QString>
#include <QVariant>

#include <qtExports.h>

/// Methods for encoding JSON.
///
/// This namespace provides methods for converting C++ structures into JSON
/// strings. An arbitrary value is represented as a ::QVariant, which may be
/// a JSON array or object, or a simple value type. An array is represented
/// as a list of values (i.e. ::QList&lt;::QVariant&gt;), while an object is
/// represented as an ordered set of keys and values (i.e.
/// ::QMap&lt;::QString, ::QVariant&gt;); both of which are themselves value
/// types.
///
/// Additionally, a QVariant containing a qtJson::JsonData is assumed to
/// represent already-encoded JSON data. This is intended as a convenience for
/// users that wish to pre-encode JSON objects and later use the encoded
/// objects in other objects or arrays.
///
/// Supported simple value types are q[u]int{8,16,32,64}, float, double,
/// boolean, and QString. Additionally, an invalid QVariant is encoded as
/// "null". Unsupported types are encoded to empty JsonData.
///
/// JSON representations are stored in UTF-8.
namespace qtJson
{
  /// Convenience typedef for a JSON value.
  typedef QVariant Value;

  /// Convenience typedef for a JSON array.
  typedef QVariantList Array;

  /// Convenience typedef for a JSON object.
  typedef QVariantMap Object;

  /// Convenience typedef for an encoded JSON string.
  typedef QByteArray JsonData;

  /// Encode a string into JSON representation.
  ///
  /// The returned string is enclosed in quotes, and special characters have
  /// been escaped.
  QTE_EXPORT JsonData encode(QString);

  /// Encode an array into JSON representation.
  QTE_EXPORT JsonData encode(const Array&);

  /// Encode an object into JSON representation.
  QTE_EXPORT JsonData encode(const Object&);

  /// Encode a value into JSON representation.
  ///
  /// This encodes a value into a JSON string.
  ///
  /// \li If the value is invalid (QVariant::isNull returns \c true), the
  ///     result is "null".
  /// \li If the value is a boolean, the result is "true" or "false".
  /// \li If the value is a numeric type, result is equivalent to
  ///     QByteArray::number(value) for integers and finite reals, "null" for
  ///     infinities or NaN's.
  /// \li If the value is a QString, qtJson::Array, or qtJson::Object, the
  ///     result is equivalent to calling the respective overload of
  ///     \c qtJson::encode.
  /// \li If the value is a JsonData, it is returned unaltered.
  /// \li For all other values, the result is an empty JsonData.
  QTE_EXPORT JsonData encode(const Value&);
}

#endif
