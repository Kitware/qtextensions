// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  return {ss.data(), static_cast<int>(ss.size())};
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
inline std::string stdString(const QStringRef& qs)
{
  const QByteArray data = qs.toLocal8Bit();
  return std::string(data.constData(), static_cast<size_t>(data.size()));
}

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
