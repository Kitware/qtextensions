/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QDir>
#include <QScopedArrayPointer>

static const int MAX_RETRIES = 500;

#ifdef Q_OS_WIN

  #include <QFileInfo>
  #include <QRegExp>
  #include <QUuid>

  #include <io.h>
  #include <fcntl.h>

#else

  #include <cstdio>
  #include <cstdlib>
  #include <unistd.h>

#endif

#include <sys/stat.h>

#include <cerrno>

#include "qtTemporaryFile.h"

QTE_IMPLEMENT_D_FUNC(qtTemporaryFile)

//-----------------------------------------------------------------------------
class qtTemporaryFilePrivate
{
public:
  QString templatePath;
};

//-----------------------------------------------------------------------------
qtTemporaryFile::qtTemporaryFile() : d_ptr(new qtTemporaryFilePrivate)
{
  this->setTemplateName(".qte_temp.XXXXXX");
}

//-----------------------------------------------------------------------------
qtTemporaryFile::qtTemporaryFile(const QString& templateName)
  : d_ptr(new qtTemporaryFilePrivate)
{
  (QDir::isAbsolutePath(templateName)
   ? this->setTemplatePath(templateName)
   : this->setTemplateName(templateName));
}

//-----------------------------------------------------------------------------
qtTemporaryFile::~qtTemporaryFile()
{
  QTE_D(qtTemporaryFile);
  delete d;
}

//-----------------------------------------------------------------------------
QString qtTemporaryFile::templatePath() const
{
  QTE_D_CONST(qtTemporaryFile);
  return d->templatePath;
}

//-----------------------------------------------------------------------------
void qtTemporaryFile::setTemplateName(const QString& name)
{
  this->setTemplatePath(QDir::tempPath() + '/' + name);
}

//-----------------------------------------------------------------------------
void qtTemporaryFile::setTemplatePath(const QString& path)
{
  QTE_D(qtTemporaryFile);
  d->templatePath = path;
}

//-----------------------------------------------------------------------------
bool qtTemporaryFile::open()
{
  return this->open(QIODevice::ReadWrite);
}

//-----------------------------------------------------------------------------
#ifdef Q_OS_WIN
namespace
{

QString mktemp(QString templatePath)
{
  QFileInfo fi(templatePath);
  QDir dir(fi.path());
  if (!dir.exists())
    {
    return QString();
    }

  QString fileName = fi.fileName();
  QString prefix = dir.canonicalPath() + '/' +
                   fileName.left(fileName.count() - 6);
  QString seed = QUuid::createUuid().toString().toLower();
  seed = seed.remove(QRegExp("[^0123456789abcdef]"));
  QByteArray suffix = QByteArray::fromHex(seed.toAscii()).toBase64();
  suffix = suffix.left(6).replace('/', ',');
  return prefix + QString::fromAscii(suffix.toLower());
}

}
#endif

//-----------------------------------------------------------------------------
bool qtTemporaryFile::open(OpenMode flags)
{
  if ((flags & QIODevice::ReadWrite) != QIODevice::ReadWrite)
    {
    this->setErrorString("Bad mode");
    return false;
    }

  QTE_D(qtTemporaryFile);

  if (!d->templatePath.endsWith("XXXXXX"))
    {
    this->setErrorString("Bad template");
    return false;
    }

  int fd = -1;

#ifdef Q_OS_WIN

  int tries = MAX_RETRIES;
  while (--tries)
    {
    // Generate name of a non-existing file
    QString t = mktemp(d->templatePath);
    if (t.isEmpty())
      {
      break;
      }

    // Try to open the file:
    // - _O_RDWR | _O_BINARY should be obvious
    // - _O_CREAT | _O_EXCL specifies it must not exist
    // - _O_SHORT_LIVED says to keep it in cache only, if possible
    // - _O_TEMPORARY says to delete it on close; best we can do on Windows
    // - _O_NOINHERIT says "prevent creation of a shared file descriptor"
    int oflags = _O_RDWR | _O_BINARY | _O_CREAT | _O_EXCL |
                 _O_SHORT_LIVED | _O_TEMPORARY | _O_NOINHERIT;
    errno_t e = _wsopen_s(&fd, t.toStdWString().c_str(),
                          oflags, _SH_DENYRW, _S_IWRITE | _S_IREAD);

    if (e == EEXIST)
      {
      fd = -1;
      continue;
      }
    else if (e != 0)
      {
      // Unable to open the file; try again
      this->setErrorString("Failed to open temporary file: " +
                           qt_error_string(errno));
      return false;
      }
    else
      {
      break;
      }
    }

  // No dice...
  if (fd < 0)
    {
    this->setErrorString("Unable to create temporary file name");
    return false;
    }

#else

  // Create the temporary file
  QByteArray path = d->templatePath.toLocal8Bit();
  fd = mkstemp(path.data());

  // Check for failure, and set error string accordingly
  if (fd < 0)
    {
    this->setErrorString("Failed to open temporary file: " +
                         qt_error_string(errno));
    return false;
    }

  // Remove the file immediately. This way, if anything goes wrong, at worst we
  // litter an empty file. By removing it immediately, we ensure that the file
  // system will garbage collect it when we die (even abnormally), and it also
  // serves as an additional measure of security.
  if (unlink(path.constData()) < 0)
    {
    this->setErrorString("Failed to unlink temporary file: " +
                         qt_error_string(errno));
    return false;
    }

  // Set the permissions so that the file is not writable by other users...
  // just in case someone is able to recover a link to the descriptor (the
  // specification of mkstemp does not include the mode with which the file is
  // created)
  if (fchmod(fd, S_IRUSR | S_IWUSR) < 0)
    {
    this->setErrorString("Failed to change permissions of temporary file: " +
                         qt_error_string(errno));
    return false;
    }

  // Now that the file is as safe as we can make it, ensure it is empty and we
  // are not seeing anything that might possibly have been written to it by a
  // malicious user
  if (ftruncate(fd, 0) < 0)
    {
    this->setErrorString("Failed to truncate temporary file: " +
                         qt_error_string(errno));
    return false;
    }

#endif

  // At this point, the file descriptor has been opened successfully, so now
  // all we need to do is wrap the fd with QFile so that we can use it normally
  return (QFile::open(fd, flags));
}
