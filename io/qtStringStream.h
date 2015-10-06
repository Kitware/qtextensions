/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
