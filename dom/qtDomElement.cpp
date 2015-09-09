/*ckwg +5
 * Copyright 2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtDomElement.h"

//-----------------------------------------------------------------------------
qtDomElement::qtDomElement() : Document(*reinterpret_cast<QDomDocument*>(0))
{
}

//-----------------------------------------------------------------------------
qtDomElement::qtDomElement(
  QDomDocument& doc, const QString& type, const QString& id) :
  QDomElement(doc.createElement(type)), Document(doc)
{
  if (!id.isEmpty())
    {
    this->QDomElement::setAttribute("id", id);
    }
}

//-----------------------------------------------------------------------------
qtDomElement::qtDomElement(QDomDocument& doc, const QDomElement& other) :
  QDomElement(other), Document(doc)
{
}

//-----------------------------------------------------------------------------
qtDomElement& qtDomElement::add(const QDomNode& node)
{
  this->appendChild(node);
  return *this;
}

//-----------------------------------------------------------------------------
qtDomElement& qtDomElement::addText(const QString& text)
{
  this->appendChild(this->Document.createTextNode(text));
  return *this;
}

//-----------------------------------------------------------------------------
#define SET_ATTRIBUTE(type) \
qtDomElement& qtDomElement::setAttribute(const QString& name, type value) \
{ \
  this->QDomElement::setAttribute(name, value); \
  return *this; \
}

SET_ATTRIBUTE(const QString&)
SET_ATTRIBUTE(int)
SET_ATTRIBUTE(uint)
SET_ATTRIBUTE(qlonglong)
SET_ATTRIBUTE(qulonglong)
SET_ATTRIBUTE(float)
SET_ATTRIBUTE(double)
