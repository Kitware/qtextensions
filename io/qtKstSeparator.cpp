/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtKstSeparator.h"

#include <QRegExp>

QTE_IMPLEMENT_D_FUNC(qtKstSeparator)

//BEGIN qtKstSeparatorPrivate and implementation subclasses

//-----------------------------------------------------------------------------
class qtKstSeparatorPrivate
{
public:
  static qtKstSeparatorPrivate* create(const QRegExp&);
  virtual qtKstSeparatorPrivate* clone() const = 0;

  virtual ~qtKstSeparatorPrivate() {};

  virtual bool matches(const QString& data, int pos) = 0;
  virtual int matchedLength() const = 0;

  const QString pattern;

protected:
  qtKstSeparatorPrivate(const QString& pattern) : pattern(pattern) {}
};

//-----------------------------------------------------------------------------
class qtKstBasicSeparator : public qtKstSeparatorPrivate
{
public:
  qtKstBasicSeparator(const QString& pattern)
    : qtKstSeparatorPrivate(pattern), patternLength(pattern.length()) {}

  virtual qtKstSeparatorPrivate* clone() const
    { return new qtKstBasicSeparator(*this); }

  virtual bool matches(const QString& data, int pos)
    { return data.mid(pos, this->patternLength) == this->pattern; }

  virtual int matchedLength() const
    { return this->patternLength; }

  const int patternLength;
};

//-----------------------------------------------------------------------------
class qtKstRegExpSeparator : public qtKstSeparatorPrivate
{
public:
  qtKstRegExpSeparator(const QRegExp& exp);

  virtual qtKstSeparatorPrivate* clone() const
    { return new qtKstRegExpSeparator(*this); }

  virtual bool matches(const QString& data, int pos)
    { return exp.indexIn(data, pos, QRegExp::CaretAtOffset) == pos; }

  virtual int matchedLength() const
    { return exp.matchedLength(); }

  QRegExp exp;
};

//-----------------------------------------------------------------------------
qtKstRegExpSeparator::qtKstRegExpSeparator(const QRegExp& exp)
  : qtKstSeparatorPrivate(exp.pattern()), exp(exp)
{
  if (!this->exp.pattern().startsWith('^'))
    {
    this->exp.setPattern('^' + this->exp.pattern());
    }
}

//-----------------------------------------------------------------------------
qtKstSeparatorPrivate* qtKstSeparatorPrivate::create(const QRegExp& exp)
{
  if (exp.patternSyntax() == QRegExp::FixedString)
    {
    return new qtKstBasicSeparator(exp.pattern());
    }
  return new qtKstRegExpSeparator(exp);
}

//END qtKstSeparatorPrivate and implementation subclasses

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtKstSeparator

//-----------------------------------------------------------------------------
qtKstSeparator::qtKstSeparator(const QRegExp& regexp)
  : d_ptr(qtKstSeparatorPrivate::create(regexp))
{
}

//-----------------------------------------------------------------------------
qtKstSeparator::qtKstSeparator(const qtKstSeparator& other)
  : d_ptr(other.d_func()->clone())
{
}

//-----------------------------------------------------------------------------
qtKstSeparator::~qtKstSeparator()
{
  QTE_D(qtKstSeparator);
  delete d;
}

//-----------------------------------------------------------------------------
bool qtKstSeparator::matches(const QString& data, int pos)
{
  QTE_D(qtKstSeparator);
  return d->matches(data, pos);
}

//-----------------------------------------------------------------------------
int qtKstSeparator::matchedLength() const
{
  QTE_D_CONST(qtKstSeparator);
  return d->matchedLength();
}

//-----------------------------------------------------------------------------
QString qtKstSeparator::pattern() const
{
  QTE_D_CONST(qtKstSeparator);
  return d->pattern;
}

//END qtKstSeparator