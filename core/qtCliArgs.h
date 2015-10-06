/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtCliArgs_h
#define __qtCliArgs_h

#include <QString>

#include "qtCliOptions.h"
#include "qtGlobal.h"

class qtCliOptions;

class qtCliArgsPrivate;

class QTE_EXPORT qtCliArgs
{
public:
  enum ParseOption
    {
    IgnoreRequired = 0x1,
    };
  Q_DECLARE_FLAGS(ParseOptions, ParseOption)

  qtCliArgs(int argc, char** argv);
  ~qtCliArgs();

  void addOptions(const qtCliOptions&, QString group = QString(),
                  bool includeWithCommon = true);
  void addNamedArguments(const qtCliOptions&);

  bool parse(ParseOptions = static_cast<qtCliArgs::ParseOptions>(0));
  void parseOrDie(int exitCode = 1);
  void parseOrDie(ParseOption, int exitCode = 1);
  void parseOrDie(ParseOptions, int exitCode = 1);

  void parseError() const;
  void shortUsage() const;
  void usage(QString group = QString()) const;
  void usageError(const QString& message) const;

  bool isSet(const QString& name) const;
  QString value(const QString& name) const;
  QStringList values(const QString& name) const;

  int count() const;
  QString arg(int index) const;
  QStringList args() const;

  /// Return the file name of the executing process.
  /// This method returns the file name of current process executable, without
  /// path. This is the name displayed with usage information, which may be
  /// useful to users for displaying error messages.
  QString executableName() const;

  /// Return argc suitable for passing to QCoreApplication constructor.
  /// WARNING: The return value points to internal storage that may be different
  /// each time this method is called. Callers must ensure that this object
  /// remains in scope as long as any other objects refer to the returned value
  /// (typically, by ensuring that this object remains in scope for the life of
  /// the QCoreApplication).
  int& qtArgc();

  /// Return argv suitable for passing to QCoreApplication constructor.
  /// WARNING: The return value points to internal storage that may be different
  /// each time this method is called. Callers must ensure that this object
  /// remains in scope as long as any other objects refer to the returned value
  /// (typically, by ensuring that this object remains in scope for the life of
  /// the QCoreApplication).
  char** qtArgv();

  /// Join a list of words, wrapped to the TTY width, indenting by \p indent
  /// after each new line, wrapping as if the first line starts \p prefixSize
  /// characters after the first column
  static QString joinWrapped(QStringList words, int indent,
                             int prefixSize = 0);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtCliArgs);

private:
  QTE_DECLARE_PRIVATE(qtCliArgs);
  QTE_DISABLE_COPY(qtCliArgs);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(qtCliArgs::ParseOptions)

#endif
