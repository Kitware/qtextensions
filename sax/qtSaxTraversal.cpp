// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtSaxTraversal.h"

//-----------------------------------------------------------------------------
qtSaxTraversal::qtSaxTraversal(QXmlStreamReader& stream) :
  Stream(stream), Depth(1), Flag(false), Offset(stream.characterOffset())
{
}

//-----------------------------------------------------------------------------
QXmlStreamReader::TokenType qtSaxTraversal::next()
{
  if (this->Stream.atEnd())
    {
    this->Depth = -1;
    return QXmlStreamReader::NoToken;
    }

  // If someone else advanced the stream, process the current token; this is
  // required so that nested loops unwind correctly as we will see the start
  // token, but not otherwise the end token
  const bool advanced = (this->Stream.characterOffset() != this->Offset);
  const QXmlStreamReader::TokenType token =
    (advanced ? this->Stream.tokenType() : this->Stream.readNext());

  switch (token)
    {
    case QXmlStreamReader::StartElement:
      ++this->Depth;
      break;
    case QXmlStreamReader::EndElement:
      --this->Depth;
      break;
    default:
      break;
    }

  this->Offset = this->Stream.characterOffset();

  return token;
}
