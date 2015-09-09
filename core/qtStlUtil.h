/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtStlUtil_h
#define __qtStlUtil_h

#include <string>
#include <vector>

#include <QList>
#include <QString>
#include <QUrl>

#include <qtExports.h>

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
  return std::string(data.constData(), data.size());
}

//-----------------------------------------------------------------------------
#if QT_VERSION < 0x040800
extern QTE_EXPORT std::string stdString(const QStringRef& qs);
#else
inline std::string stdString(const QStringRef& qs)
{
  const QByteArray data = qs.toLocal8Bit();
  return std::string(data.constData(), data.size());
}
#endif

//-----------------------------------------------------------------------------
inline std::string stdString(const QUrl& qu)
{
  const QByteArray data = qu.toEncoded();
  return std::string(data.constData(), data.size());
}

//-----------------------------------------------------------------------------
template <typename T>
inline QList<T> qtList(const std::vector<T>& in)
{
  const size_t k = in.size();

  QList<T> out;
  out.reserve(static_cast<int>(k));

  for (size_t n = 0; n < k; ++n)
    {
    out.append(in[n]);
    }

  return out;
}

#endif
