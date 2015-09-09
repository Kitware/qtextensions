/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
  qtCliOption& add(QString name, QString description = QString(),
                   QString defaultValue = QString(),
                   Flags = static_cast<qtCliOption::Flags>(0));

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
