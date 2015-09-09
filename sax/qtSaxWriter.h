/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtSaxWriter_h
#define __qtSaxWriter_h

#include "../core/qtGlobal.h"

#include "qtSaxNamespace.h"

#include <QScopedPointer>
#include <QString>

class QByteArray;
class QIODevice;

class qtSaxNode;

class qtSaxWriterPrivate;

//-----------------------------------------------------------------------------
class QTE_EXPORT qtSaxWriter
{
public:
  explicit qtSaxWriter(QIODevice* device);
  explicit qtSaxWriter(QString* buffer);
  explicit qtSaxWriter(QByteArray* buffer);
  virtual ~qtSaxWriter();

  qtSaxWriter& start(const QString& version = QString("1.0"));
  void end();

  qtSaxWriter& operator<<(const QString&);
  qtSaxWriter& operator<<(const qtSaxNode&);
  qtSaxWriter& operator<<(qtSax::Directive);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtSaxWriter)

private:
  QTE_DECLARE_PRIVATE(qtSaxWriter)
};

#endif
