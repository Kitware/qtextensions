/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtSaxNodes.h"

#include <QRegExp>
#include <QXmlStreamReader>

//BEGIN qtSaxElement

//-----------------------------------------------------------------------------
class qtSaxElementPrivate
{
public:
    qtSaxElementPrivate(QString const& name) : name(name) {}

    QString const name;
};
QTE_IMPLEMENT_D_FUNC(qtSaxElement)

//-----------------------------------------------------------------------------
qtSaxElement::qtSaxElement(QString const& name)
    : d_ptr(new qtSaxElementPrivate(name))
{
}

//-----------------------------------------------------------------------------
qtSaxElement::~qtSaxElement()
{
}

//-----------------------------------------------------------------------------
void qtSaxElement::write(QXmlStreamWriter& stream) const
{
    QTE_D();
    stream.writeStartElement(d->name);
}

//END qtSaxElement

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSaxEmptyElement

//-----------------------------------------------------------------------------
qtSaxEmptyElement::qtSaxEmptyElement(QString const& name)
    : qtSaxElement(name)
{
}

//-----------------------------------------------------------------------------
qtSaxEmptyElement::~qtSaxEmptyElement()
{
}

//-----------------------------------------------------------------------------
void qtSaxEmptyElement::write(QXmlStreamWriter& stream) const
{
    QTE_D();
    stream.writeEmptyElement(d->name);
}

//END qtSaxEmptyElement

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSaxAttribute

//-----------------------------------------------------------------------------
class qtSaxAttributePrivate
{
public:
    qtSaxAttributePrivate(QString const& name, QString const& value)
        : name(name), value(value) {}

    QString const name;
    QString const value;
};
QTE_IMPLEMENT_D_FUNC(qtSaxAttribute)

//-----------------------------------------------------------------------------
qtSaxAttribute::qtSaxAttribute(QString const& name, QString const& value)
    : d_ptr(new qtSaxAttributePrivate(name, value))
{
}

//-----------------------------------------------------------------------------
qtSaxAttribute::~qtSaxAttribute()
{
}

//-----------------------------------------------------------------------------
void qtSaxAttribute::write(QXmlStreamWriter& stream) const
{
    QTE_D();
    stream.writeAttribute(d->name, d->value);
}

//END qtSaxAttribute

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSaxEntity

//-----------------------------------------------------------------------------
class qtSaxEntityPrivate
{
public:
    qtSaxEntityPrivate(QString const& name) : name(name) {}

    QString const name;
};
QTE_IMPLEMENT_D_FUNC(qtSaxEntity)

//-----------------------------------------------------------------------------
qtSaxEntity::qtSaxEntity(QString const& name)
    : d_ptr(new qtSaxEntityPrivate(name))
{
}

//-----------------------------------------------------------------------------
qtSaxEntity::~qtSaxEntity()
{
}

//-----------------------------------------------------------------------------
void qtSaxEntity::write(QXmlStreamWriter& stream) const
{
    QTE_D();
    stream.writeEntityReference(d->name);
}

//END qtSaxEntity

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSaxText

//-----------------------------------------------------------------------------
class qtSaxTextPrivate
{
public:
    qtSaxTextPrivate(QString const& text, qtSaxText::TextType type)
        : text(text), type(type) {}

    QString const text;
    qtSaxText::TextType const type;
};
QTE_IMPLEMENT_D_FUNC(qtSaxText)

//-----------------------------------------------------------------------------
qtSaxText::qtSaxText(QString const& text, qtSaxText::TextType type)
    : d_ptr(new qtSaxTextPrivate(text, type))
{
}

//-----------------------------------------------------------------------------
qtSaxText::~qtSaxText()
{
}

//-----------------------------------------------------------------------------
void qtSaxText::write(QXmlStreamWriter& stream) const
{
    QTE_D();

    if (d->type == TextWithEntities)
    {
        auto text = d->text;
        auto reEntityRef = QRegExp("&(?!\\d)([\\w:][\\w:-.]+);");

        while (!text.isEmpty())
        {
            auto const i = reEntityRef.indexIn(text);

            if (i != 0)
                stream.writeCharacters(text.left(i));

            if (i < 0)
                break;

            stream.writeEntityReference(reEntityRef.cap(1));
            text = text.mid(i + reEntityRef.matchedLength());
        }
    }
    else
    {
        stream.writeCharacters(d->text);
    }
}

//END qtSaxText
