/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtCliOption.h"

#include <QRegExp>
#include <QStringList>

#include "qtCliArgs.h"

QTE_IMPLEMENT_D_FUNC(qtCliOption)

//-----------------------------------------------------------------------------
class qtCliOptionPrivate
{
public:
  QStringList shortNames, longNames;
  QStringList values;
  QString defaultValue;
  QString argText, description;
  bool required;
  bool allowEmpty;
  bool namedList;

  QString usage(const QString& namesText) const;
};

//-----------------------------------------------------------------------------
QString qtCliOptionPrivate::usage(const QString& namesText) const
{
  // Generate word list for option description
  QStringList descWords;
  descWords.append(this->description.split(QRegExp("\\s+")));
  if (this->required)
    {
    if (!this->defaultValue.isEmpty())
      {
      descWords.append("(required;");
      descWords.append(QString("default '%1')").arg(this->defaultValue));
      }
    else
      {
      descWords.append("(required)");
      }
    }
  else if (!this->defaultValue.isEmpty())
    {
    descWords.append(QString("(default '%1')").arg(this->defaultValue));
    }

  // Join description words to wrapped text
  QString descText =
    qtCliArgs::joinWrapped(descWords, 28, qMax(28, namesText.length() + 3));

  // Combine
  return QString("  %1 ").arg(namesText, -25) + descText;
}

//-----------------------------------------------------------------------------
qtCliOption::qtCliOption() : d_ptr(new qtCliOptionPrivate)
{
  QTE_D(qtCliOption);
  d->required = false;
  d->allowEmpty = false;
  d->namedList = false;
}

//-----------------------------------------------------------------------------
qtCliOption::qtCliOption(const qtCliOption& other)
  : d_ptr(new qtCliOptionPrivate(*other.d_func()))
{
}

//-----------------------------------------------------------------------------
qtCliOption::~qtCliOption()
{
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOption::operator=(const qtCliOption& other)
{
  QTE_D(qtCliOption);
  *d = *other.d_func();
  return *this;
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOption::add(const QString& name, Flags flags)
{
  return this->add(name, QString(), QString(), flags);
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOption::add(const QString& name, const QString& description,
                              Flags flags)
{
  return this->add(name, description, QString(), flags);
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOption::add(QString name, QString description,
                              QString defaultValue, Flags flags)
{
  QTE_D(qtCliOption);

  // Extract argument text, if given
  name = name.trimmed();
  int i = name.indexOf(' ');
  if (i > 0)
    {
    d->argText = name.mid(i + 1);
    name = name.left(i);
    }

  // Use override default value for implicitly true flags (mostly so we will
  // give nice usage text)
  const bool implicitTrueFlag =
    !flags.testFlag(Short) && d->argText.isEmpty() &&
    name.length() > 3 && name.startsWith("no-");
  if (implicitTrueFlag)
    {
    defaultValue = name.mid(3);
    }

  // Add name
  (!flags.testFlag(Short) && (name.length() > 1)
   ? d->longNames.append(name)
   : d->shortNames.append(name));

  // Fill other values, if specified
  if (!description.isEmpty())
    {
    d->description = description;
    }
  if (!defaultValue.isEmpty())
    {
    d->defaultValue = defaultValue;
    }
  d->required = d->required || flags.testFlag(Required);
  d->allowEmpty = d->allowEmpty || flags.testFlag(AllowEmpty);
  d->namedList = d->namedList || flags.testFlag(NamedList);

  // Return self so aliases can be added
  return *this;
}

//-----------------------------------------------------------------------------
QString qtCliOption::preferredName() const
{
  QTE_D_CONST(qtCliOption);
  return (d->longNames.count() ? d->longNames[0] : d->shortNames[0]);
}

//-----------------------------------------------------------------------------
QStringList qtCliOption::shortNames() const
{
  QTE_D_CONST(qtCliOption);
  return d->shortNames;
}

//-----------------------------------------------------------------------------
QStringList qtCliOption::longNames() const
{
  QTE_D_CONST(qtCliOption);
  return d->longNames;
}

//-----------------------------------------------------------------------------
QString qtCliOption::optionUsage() const
{
  QTE_D_CONST(qtCliOption);

  // Generate text for option names
  QStringList names;
  foreach (QString sn, d->shortNames)
    names.append("-" + sn);
  foreach (QString ln, d->longNames)
    names.append("--" + ln);
  QString namesText =
    QString("%1 %2").arg(names.join(", "), d->argText).trimmed();

  // Add description and return result
  return d->usage(namesText);
}

//-----------------------------------------------------------------------------
QString qtCliOption::namedArgUsage() const
{
  QTE_D_CONST(qtCliOption);
  return d->usage(this->preferredName());
}

//-----------------------------------------------------------------------------
bool qtCliOption::isFlag() const
{
  QTE_D_CONST(qtCliOption);
  return d->argText.isEmpty();
}

//-----------------------------------------------------------------------------
bool qtCliOption::isRequired() const
{
  QTE_D_CONST(qtCliOption);
  return d->required;
}

//-----------------------------------------------------------------------------
bool qtCliOption::isNamedList() const
{
  QTE_D_CONST(qtCliOption);
  return d->namedList;
}

//-----------------------------------------------------------------------------
QString qtCliOption::valueName() const
{
  QTE_D_CONST(qtCliOption);
  return d->argText;
}

//-----------------------------------------------------------------------------
bool qtCliOption::isSet() const
{
  QTE_D_CONST(qtCliOption);

  if (this->isFlag())
    {
    // Implicitly true options are handled specially; if no value is set, don't
    // treat a default of <the flag name without 'no-'> as set
    const QString n = this->preferredName();
    if (n.startsWith("no-"))
      {
      if (d->values.isEmpty() && d->defaultValue == n.mid(3))
        {
        return false;
        }
      }
    // Otherwise, for flags, we only care about the value()... if it is empty,
    // the flag is not set...
    const QString v = this->value();
    if (v.isEmpty())
      {
      return false;
      }
    // ...or if value() is any of a set of values that mean 'not set', then the
    // flag is not set; for any other non-empty value(), it is considered set
    const QString vl = v.toLower();
    return !(vl == "0" || vl == "no" || vl == "false" || vl == "off");
    }
  else
    {
    // For non-flags, if it is required and AllowEmpty was not specified, it
    // is set if the value (which may be a default value) is non-empty...
    if (d->required && !d->allowEmpty)
      {
      return !this->value().isEmpty();
      }
    // ...otherwise it is set if any value (even an explicitly empty value) was
    // given, or if the default value is non-empty
    return !(d->values.isEmpty() && d->defaultValue.isEmpty());
    }
}

//-----------------------------------------------------------------------------
QString qtCliOption::value() const
{
  QTE_D_CONST(qtCliOption);
  return d->values.isEmpty() ? d->defaultValue : d->values.last();
}

//-----------------------------------------------------------------------------
void qtCliOption::setValue(const QString& value)
{
  QTE_D(qtCliOption);
  d->values.clear();
  d->values.append(value);
}

//-----------------------------------------------------------------------------
QStringList qtCliOption::values() const
{
  QTE_D_CONST(qtCliOption);
  if (d->values.isEmpty() && !d->defaultValue.isEmpty())
    {
    QStringList result;
    result.append(d->defaultValue);
    return result;
    }
  return d->values;
}

//-----------------------------------------------------------------------------
void qtCliOption::appendValue(const QString& value)
{
  QTE_D(qtCliOption);
  d->values.append(value);
}
