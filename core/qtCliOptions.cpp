/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtCliOptions.h"

QTE_IMPLEMENT_D_FUNC(qtCliOptions)

//-----------------------------------------------------------------------------
class qtCliOptionsPrivate
{
public:
  QList<qtCliOption> options;
};

//-----------------------------------------------------------------------------
qtCliOptions::qtCliOptions() : d_ptr(new qtCliOptionsPrivate)
{
}

//-----------------------------------------------------------------------------
qtCliOptions::qtCliOptions(const qtCliOptions& other)
  : d_ptr(new qtCliOptionsPrivate(*other.d_func()))
{
}

//-----------------------------------------------------------------------------
qtCliOptions::~qtCliOptions()
{
}

//-----------------------------------------------------------------------------
qtCliOptions& qtCliOptions::operator=(const qtCliOptions& other)
{
  QTE_D(qtCliOptions);
  *d = *other.d_func();
  return *this;
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOptions::add(const QString& name, qtCliOption::Flags flags)
{
  return this->add(name, QString(), QString(), flags);
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOptions::add(const QString& name, const QString& description,
                               qtCliOption::Flags flags)
{
  return this->add(name, description, QString(), flags);
}

//-----------------------------------------------------------------------------
qtCliOption& qtCliOptions::add(const QString& name, QString description,
                               QString defaultValue, qtCliOption::Flags flags)
{
  QTE_D(qtCliOptions);
  d->options.append(qtCliOption());
  qtCliOption& option = d->options.last();
  return option.add(name, description, defaultValue, flags);
}

//-----------------------------------------------------------------------------
bool qtCliOptions::isEmpty() const
{
  QTE_D_CONST(qtCliOptions);
  return d->options.isEmpty();
}

//-----------------------------------------------------------------------------
QList<qtCliOption> qtCliOptions::options() const
{
  QTE_D_CONST(qtCliOptions);
  return d->options;
}
