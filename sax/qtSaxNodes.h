/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
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
class qtSaxNode
{
public:
  virtual ~qtSaxNode() {}

  virtual void write(QXmlStreamWriter&) const = 0;
};

//-----------------------------------------------------------------------------
class QTE_EXPORT qtSaxElement : public qtSaxNode
{
public:
  explicit qtSaxElement(const QString& name);
  virtual ~qtSaxElement();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtSaxElement)
  QTE_DECLARE_PRIVATE(qtSaxElement)

  virtual void write(QXmlStreamWriter&) const QTE_OVERRIDE;
};

//-----------------------------------------------------------------------------
class QTE_EXPORT qtSaxEmptyElement : public qtSaxElement
{
public:
  explicit qtSaxEmptyElement(const QString& name);
  virtual ~qtSaxEmptyElement();

protected:
  virtual void write(QXmlStreamWriter&) const QTE_OVERRIDE;
};

//-----------------------------------------------------------------------------
class QTE_EXPORT qtSaxAttribute : public qtSaxNode
{
public:
  explicit qtSaxAttribute(const QString& name, const QString& value);
  virtual ~qtSaxAttribute();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtSaxAttribute)
  QTE_DECLARE_PRIVATE(qtSaxAttribute)

  virtual void write(QXmlStreamWriter&) const QTE_OVERRIDE;
};

#endif
