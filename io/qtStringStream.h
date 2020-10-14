// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtStringStream_h
#define __qtStringStream_h

#include <QString>
#include <QTextStream>

class qtStringStream : public QTextStream
{
public:
  qtStringStream() { this->setString(&this->buffer); }

protected:
  QString buffer;

private:
  QTE_DISABLE_COPY(qtStringStream)
};

#endif
