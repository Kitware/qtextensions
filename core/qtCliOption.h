// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtCliOption_h
#define __qtCliOption_h

#include <QScopedPointer>
#include <QString>

#include "qtGlobal.h"

class qtCliOptionPrivate;

class QTE_EXPORT qtCliOption
{
public:
  enum Flag
    {
    Required = 0x1,
    AllowEmpty = 0x2,
    NamedList = 0x8,
    Short = 0x10
    };
  Q_DECLARE_FLAGS(Flags, Flag)

  qtCliOption();
  qtCliOption(const qtCliOption&);
  ~qtCliOption();

  qtCliOption& operator=(const qtCliOption&);

  qtCliOption& add(const QString& name, Flags);
  qtCliOption& add(const QString& name, const QString& description, Flags);
  qtCliOption& add(QString name, QString description = {},
                   QString defaultValue = {}, Flags = {});

  QString preferredName() const;
  QStringList shortNames() const;
  QStringList longNames() const;

  QString optionUsage() const;
  QString namedArgUsage() const;

  bool isFlag() const;
  bool isRequired() const;
  bool isNamedList() const;
  QString valueName() const;

  bool isSet() const;
  QString value() const;
  void setValue(const QString&);
  QStringList values() const;
  void appendValue(const QString&);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtCliOption);

private:
  QTE_DECLARE_PRIVATE(qtCliOption);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(qtCliOption::Flags)

#endif
