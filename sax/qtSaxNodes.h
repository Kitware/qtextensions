/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtSaxNodes_h
#define __qtSaxNodes_h

#include "../core/qtGlobal.h"

#include <QScopedPointer>

class QString;
class QXmlStreamWriter;

class qtSaxWriter;

class qtSaxDocumentPrivate;
class qtSaxElementPrivate;
class qtSaxAttributePrivate;

//-----------------------------------------------------------------------------
/// Base class for SAX writer nodes
///
/// This class provides the generic interface for XML nodes that may be written
/// via qtSaxWriter. It is not meant to be used directly.
class qtSaxNode
{
public:
    virtual ~qtSaxNode() {}

    virtual void write(QXmlStreamWriter&) const = 0;
};

//-----------------------------------------------------------------------------
/// SAX writer node representing an element
///
/// This class represents an XML element. The element may have attributes,
/// child elements, and text. The node must be closed using qtSax::EndElement
/// or qtSaxWriter::end().
///
/// \sa QXmlStreamWriter::writeElement
class QTE_EXPORT qtSaxElement : public qtSaxNode
{
public:
    explicit qtSaxElement(QString const& name);
    virtual ~qtSaxElement();

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtSaxElement)
    QTE_DECLARE_PRIVATE(qtSaxElement)

    virtual void write(QXmlStreamWriter&) const QTE_OVERRIDE;
};

//-----------------------------------------------------------------------------
/// SAX writer node representing a self-closing element
///
/// This class represents a self-closing XML element. The element may have
/// attributes, but may not have children or text. The element is automatically
/// closed when a new element or text is written.
///
/// \sa QXmlStreamWriter::writeEmptyElement
class QTE_EXPORT qtSaxEmptyElement : public qtSaxElement
{
public:
    explicit qtSaxEmptyElement(QString const& name);
    virtual ~qtSaxEmptyElement();

protected:
    virtual void write(QXmlStreamWriter&) const QTE_OVERRIDE;
};

//-----------------------------------------------------------------------------
/// SAX writer node representing an element
///
/// This class represents an XML element attribute. Attributes must be written
/// immediately following a qtSaxElement or qtSaxEmptyElement.
///
/// \sa QXmlStreamWriter::writeAttribute
class QTE_EXPORT qtSaxAttribute : public qtSaxNode
{
public:
    explicit qtSaxAttribute(QString const& name, QString const& value);
    virtual ~qtSaxAttribute();

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtSaxAttribute)
    QTE_DECLARE_PRIVATE(qtSaxAttribute)

    virtual void write(QXmlStreamWriter&) const QTE_OVERRIDE;
};

#endif
