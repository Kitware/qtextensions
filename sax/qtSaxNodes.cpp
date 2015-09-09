/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtSaxNodes.h"

#include <QXmlStreamReader>

//BEGIN qtSaxElement

//-----------------------------------------------------------------------------
class qtSaxElementPrivate
{
public:
  qtSaxElementPrivate(const QString& name) : Name(name) {}

  QString Name;
};
QTE_IMPLEMENT_D_FUNC(qtSaxElement)

//-----------------------------------------------------------------------------
qtSaxElement::qtSaxElement(const QString& name) :
  d_ptr(new qtSaxElementPrivate(name))
{
}

//-----------------------------------------------------------------------------
qtSaxElement::~qtSaxElement()
{
}

//-----------------------------------------------------------------------------
void qtSaxElement::write(QXmlStreamWriter& stream) const
{
  QTE_D_CONST(qtSaxElement);
  stream.writeStartElement(d->Name);
}

//END qtSaxElement

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSaxEmptyElement

//-----------------------------------------------------------------------------
qtSaxEmptyElement::qtSaxEmptyElement(const QString& name) :
  qtSaxElement(name)
{
}

//-----------------------------------------------------------------------------
qtSaxEmptyElement::~qtSaxEmptyElement()
{
}

//-----------------------------------------------------------------------------
void qtSaxEmptyElement::write(QXmlStreamWriter& stream) const
{
  QTE_D_CONST(qtSaxElement);
  stream.writeEmptyElement(d->Name);
}

//END qtSaxEmptyElement

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSaxAttribute

//-----------------------------------------------------------------------------
class qtSaxAttributePrivate
{
public:
  qtSaxAttributePrivate(const QString& name, const QString& value) :
    Name(name), Value(value) {}

  QString Name;
  QString Value;
};
QTE_IMPLEMENT_D_FUNC(qtSaxAttribute)

//-----------------------------------------------------------------------------
qtSaxAttribute::qtSaxAttribute(const QString& name, const QString& value) :
  d_ptr(new qtSaxAttributePrivate(name, value))
{
}

//-----------------------------------------------------------------------------
qtSaxAttribute::~qtSaxAttribute()
{
}

//-----------------------------------------------------------------------------
void qtSaxAttribute::write(QXmlStreamWriter& stream) const
{
  QTE_D_CONST(qtSaxAttribute);
  stream.writeAttribute(d->Name, d->Value);
}

//END qtSaxAttribute
