/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtJson.h"

#include "../core/qtMath.h"

namespace // anonymous
{

//-----------------------------------------------------------------------------
qtJson::JsonData join(QList<qtJson::JsonData> list, char prefix, char suffix)
{
  qtJson::JsonData result;

  result += prefix;
  for (int i = 0, k = list.count(); i < k; ++i)
    {
    if (i)
      {
      result += ',';
      }
    result += list[i];
    }
  result += suffix;

  return result;
}

} // namespace <anonymous>

//-----------------------------------------------------------------------------
qtJson::JsonData qtJson::encode(QString value)
{
  const int k = value.length();

  JsonData result;
  result.reserve(k + 2);

  result += "\"";
  for (int i = 0; i < k; ++i)
    {
    const QChar& c = value[i];
    if (c == '\\')
      {
      result += "\\\\";
      }
    else if (c == '\"')
      {
      result += "\\\"";
      }
    else if (!c.isPrint())
      {
      result += QString().sprintf("\\u%04x", c.unicode()).toUtf8();
      }
    else if (c.unicode() < 128)
      {
      result += c.toAscii();
      }
    else
      {
      result += QString(c).toUtf8();
      }
    }
  result += "\"";

  return result;
}

//-----------------------------------------------------------------------------
qtJson::JsonData qtJson::encode(const qtJson::Array& array)
{
  QList<JsonData> encodedValues;
  encodedValues.reserve(array.count());

  foreach (const Value& value, array)
    {
    encodedValues.append(encode(value));
    }

  return join(encodedValues, '[', ']');
}

//-----------------------------------------------------------------------------
qtJson::JsonData qtJson::encode(const qtJson::Object& object)
{
  QList<JsonData> encodedPairs;
  encodedPairs.reserve(object.count());

  const Object::const_iterator end = object.constEnd();
  for (Object::const_iterator iter = object.constBegin(); iter != end; ++iter)
    {
    encodedPairs.append(encode(iter.key()) + ':' + encode(iter.value()));
    }

  return join(encodedPairs, '{', '}');
}

//-----------------------------------------------------------------------------
qtJson::JsonData qtJson::encode(const Value& value)
{
  switch (static_cast<QMetaType::Type>(value.type()))
    {
    // Already encoded data
    case QVariant::ByteArray:
      return value.toByteArray();

    // Special values
    case QVariant::Invalid:
      return "null";
    case QVariant::Bool:
      return (value.toBool() ? "true" : "false");

    // Integer types
    case QMetaType::Char:
      return QByteArray::number(value.value<char>());
    case QMetaType::UChar:
      return QByteArray::number(value.value<uchar>());
    case QMetaType::Short:
      return QByteArray::number(value.value<short>());
    case QMetaType::UShort:
      return QByteArray::number(value.value<ushort>());
    case QVariant::Int:
      return QByteArray::number(value.toInt());
    case QVariant::UInt:
      return QByteArray::number(value.toUInt());
    case QVariant::LongLong:
      return QByteArray::number(value.toLongLong());
    case QVariant::ULongLong:
      return QByteArray::number(value.toULongLong());

    // Floating point types
    case QMetaType::Float:
      {
      const float fv = value.value<float>();
      return (qIsFinite(fv) ? QByteArray::number(fv, 'g', 8) : "null");
      }
    case QVariant::Double:
      {
      const double dv = value.toDouble();
      return (qIsFinite(dv) ? QByteArray::number(dv, 'g', 8) : "null");
      }

    // String
    case QVariant::String:
      return encode(value.toString());

    // Array or object
    case QVariant::List:
      return encode(value.toList());
    case QVariant::Map:
      return encode(value.toMap());

    // Anything else is not supported
    default:
      return {};
    }
}
