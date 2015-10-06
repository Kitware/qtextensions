/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtKstReader_h
#define __qtKstReader_h

#include <QString>
#include <QRegExp>
#include <QList>
#include <QUrl>

#include "../core/qtGlobal.h"

class qtKstReaderPrivate;

class QTE_EXPORT qtKstReader
{
public:
  qtKstReader();
  explicit qtKstReader(const QUrl& url,
                       const QRegExp& separator = defaultSeparator(),
                       const QRegExp& terminator = defaultTerminator());
  explicit qtKstReader(const QString& data,
                       const QRegExp& separator = defaultSeparator(),
                       const QRegExp& terminator = defaultTerminator());
  ~qtKstReader();

  static QRegExp defaultSeparator();
  static QRegExp defaultTerminator();

  bool isValid() const;

  int recordCount() const;
  int valueCount(int record = -1) const;
  int currentRecord() const;
  int currentValue() const;

  bool nextValue();
  bool nextRecord();
  bool seek(int record = 0, int value = 0);

  bool isEndOfRecord(int value = -1, int record = -1) const;
  bool isEndOfFile(int record = -1) const;

  bool isValueEmpty(int value = -1, int record = -1) const;
  bool isArrayEmpty(int value = -1, int record = -1) const;

  bool readInt(int& out, int value = -1, int record = -1) const;
  bool readLong(qint64& out, int value = -1, int record = -1) const;
  bool readReal(double& out, int value = -1, int record = -1) const;
  bool readString(QString& out, int value = -1, int record = -1) const;
  bool readIntArray(QList<int>& out, int value = -1, int record = -1) const;
  bool readLongArray(QList<qint64>& out, int value = -1, int record = -1) const;
  bool readRealArray(QList<double>& out, int value = -1, int record = -1) const;

  bool readArray(qtKstReader& out, int value = -1, int record = -1) const;
  bool readTable(qtKstReader& out, int value = -1, int record = -1) const;

private:
  QTE_DECLARE_PRIVATE(qtKstReader)
  QTE_DECLARE_PRIVATE_MRPTR(qtKstReader)
  QTE_DISABLE_COPY(qtKstReader)

  class Value;
  typedef QList<Value> Record;

};

#endif
