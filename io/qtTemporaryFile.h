/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
