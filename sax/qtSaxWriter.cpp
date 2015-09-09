/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtSaxWriter.h"

#include "qtSaxNodes.h"

#include <QDebug>
#include <QXmlStreamReader>

//-----------------------------------------------------------------------------
class qtSaxWriterPrivate
{
public:
  qtSaxWriterPrivate(QXmlStreamWriter* stream) : Stream(stream) {}

  const QScopedPointer<QXmlStreamWriter> Stream;
};
QTE_IMPLEMENT_D_FUNC(qtSaxWriter)

//-----------------------------------------------------------------------------
qtSaxWriter::qtSaxWriter(QIODevice* device) :
  d_ptr(new qtSaxWriterPrivate(new QXmlStreamWriter(device)))
{
}

//-----------------------------------------------------------------------------
qtSaxWriter::qtSaxWriter(QString* buffer) :
  d_ptr(new qtSaxWriterPrivate(new QXmlStreamWriter(buffer)))
{
}

//-----------------------------------------------------------------------------
qtSaxWriter::qtSaxWriter(QByteArray* buffer) :
  d_ptr(new qtSaxWriterPrivate(new QXmlStreamWriter(buffer)))
{
}

//-----------------------------------------------------------------------------
qtSaxWriter::~qtSaxWriter()
{
}

//-----------------------------------------------------------------------------
qtSaxWriter& qtSaxWriter::start(const QString& version)
{
  QTE_D(qtSaxWriter);
  d->Stream->writeStartDocument(version);
  return *this;
}

//-----------------------------------------------------------------------------
void qtSaxWriter::end()
{
  QTE_D(qtSaxWriter);
  d->Stream->writeEndDocument();
}

//-----------------------------------------------------------------------------
qtSaxWriter& qtSaxWriter::operator<<(const QString& characters)
{
  QTE_D(qtSaxWriter);
  d->Stream->writeCharacters(characters);
  return *this;
}

//-----------------------------------------------------------------------------
qtSaxWriter& qtSaxWriter::operator<<(const qtSaxNode& node)
{
  QTE_D(qtSaxWriter);
  node.write(*d->Stream);
  return *this;
}

//-----------------------------------------------------------------------------
qtSaxWriter& qtSaxWriter::operator<<(qtSax::Directive directive)
{
  QTE_D(qtSaxWriter);

  switch (directive)
    {
    case qtSax::EndElement:
      d->Stream->writeEndElement();
      break;
    default:
      qDebug() << "qtSaxWriter: warning: unrecognized directive" << directive;
      break;
    }

  return *this;
}
