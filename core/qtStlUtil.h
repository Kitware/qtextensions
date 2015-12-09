/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtStlUtil_h
#define __qtStlUtil_h

#include "qtGlobal.h"
#include "qtIndexRange.h"

#include <QList>
#include <QString>
#include <QUrl>

#include <string>
#include <vector>

//-----------------------------------------------------------------------------
inline QByteArray qtBytes(const std::string& ss)
{
  return QByteArray(ss.data(), static_cast<int>(ss.size()));
}

//-----------------------------------------------------------------------------
inline QString qtString(const std::string& ss)
{
  return QString::fromLocal8Bit(ss.data(), static_cast<int>(ss.size()));
}

//-----------------------------------------------------------------------------
inline QUrl qtUrl(const std::string& ss)
{
  return QUrl::fromEncoded(qtBytes(ss));
}

//-----------------------------------------------------------------------------
inline std::string stdString(const QString& qs)
{
  const QByteArray data = qs.toLocal8Bit();
  return std::string(data.constData(), static_cast<size_t>(data.size()));
}

//-----------------------------------------------------------------------------
#if QT_VERSION < 0x040800
extern QTE_EXPORT std::string stdString(const QStringRef& qs);
#else
inline std::string stdString(const QStringRef& qs)
{
  const QByteArray data = qs.toLocal8Bit();
  return std::string(data.constData(), static_cast<size_t>(data.size()));
}
#endif

//-----------------------------------------------------------------------------
inline std::string stdString(const QUrl& qu)
{
  const QByteArray data = qu.toEncoded();
  return std::string(data.constData(), static_cast<size_t>(data.size()));
}

//-----------------------------------------------------------------------------
template <typename T>
inline QList<T> qtList(const std::vector<T>& in)
{
  auto const k = in.size();

  QList<T> out;
  out.reserve(static_cast<int>(k));

  foreach (auto const n, qtIndexRange(k))
    out.append(in[n]);

  return out;
}

#endif
