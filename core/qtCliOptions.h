// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtCliOptions_h
#define __qtCliOptions_h

#include <QList>

#include "qtCliOption.h"
#include "qtGlobal.h"

class qtCliOptionsPrivate;

class QTE_EXPORT qtCliOptions
{
public:
  qtCliOptions();
  qtCliOptions(const qtCliOptions&);
  ~qtCliOptions();

  qtCliOptions& operator=(const qtCliOptions&);

  qtCliOption& add(const QString& name, qtCliOption::Flags);
  qtCliOption& add(const QString& name, const QString& description,
                   qtCliOption::Flags);
  qtCliOption& add(const QString& name, QString description = QString(),
                   QString defaultValue = QString(), qtCliOption::Flags = 0);

  bool isEmpty() const;
  QList<qtCliOption> options() const;

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtCliOptions);

private:
  QTE_DECLARE_PRIVATE(qtCliOptions);
};

#endif
