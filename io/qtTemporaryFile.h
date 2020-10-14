// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtTemporaryFile_h
#define __qtTemporaryFile_h

#include <QFile>

#include "../core/qtGlobal.h"

class qtTemporaryFilePrivate;

class QTE_EXPORT qtTemporaryFile : public QFile
{
public:
  qtTemporaryFile();
  explicit qtTemporaryFile(const QString& templateName);
  virtual ~qtTemporaryFile();

  bool open();
  virtual bool open(OpenMode flags) QTE_OVERRIDE;

  QString templatePath() const;
  void setTemplateName(const QString& name);
  void setTemplatePath(const QString& path);

protected:
  QTE_DECLARE_PRIVATE_PTR(qtTemporaryFile)

private:
  QTE_DECLARE_PRIVATE(qtTemporaryFile)
  QTE_DISABLE_COPY(qtTemporaryFile)
};

#endif
