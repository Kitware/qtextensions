// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
