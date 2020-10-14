// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
