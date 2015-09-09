/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtStlUtil.h"

#if QT_VERSION < 0x040800
#include <QLocale>
#include <QTextCodec>

//-----------------------------------------------------------------------------
std::string stdString(const QStringRef& qs)
{
  QTextCodec* codec = QTextCodec::codecForLocale();
  if (!codec)
    {
    codec = QTextCodec::codecForName("ISO-8859-1"); // a.k.a. latin1
    }
  const QByteArray data = codec->fromUnicode(qs.constData(), qs.length());
  return std::string(data.constData(), data.size());
}
#endif
