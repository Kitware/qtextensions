// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
