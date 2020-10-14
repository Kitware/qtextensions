// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtKstReader.h"

#include "qtKstParser.h"
#include "qtKstSeparator.h"

#include <QFile>
#include <QDebug>

#include <limits>

QTE_IMPLEMENT_D_FUNC(qtKstReader)

//BEGIN qtKstReader value

//-----------------------------------------------------------------------------
class qtKstReader::Value
{
public:
  QString value;
  qtKstReader::Record array;

  bool isEmpty() const;
  bool readString(QString& out) const;
  bool readInt(int& out) const;
  bool readLong(qint64& out) const;
  bool readReal(double& out) const;
  bool readIntArray(QList<int>& out) const;
  bool readLongArray(QList<qint64>& out) const;
  bool readRealArray(QList<double>& out) const;
};

//-----------------------------------------------------------------------------
bool qtKstReader::Value::isEmpty() const
{
  return this->array.isEmpty() && this->value.isEmpty();
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readString(QString& out) const
{
  if (this->array.count() > 1)
    {
    return false;
    }

  if (this->array.count())
    {
    return this->array[0].readString(out);
    }

  out = this->value;
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readLong(qint64& out) const
{
  QString v;
  if (!this->readString(v))
    {
    return false;
    }
  return qtKstParser::parseLong(v.toLower(), out);
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readInt(int& out) const
{
  qint64 l;
  if (!this->readLong(l))
    {
    return false;
    }
  if (l > std::numeric_limits<int>::max())
    {
    return false;
    }
  if (l < std::numeric_limits<int>::min())
    {
    return false;
    }
  out = static_cast<int>(l);
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readReal(double& out) const
{
  QString v;
  if (!this->readString(v))
    {
    return false;
    }
  return qtKstParser::parseReal(v.toLower(), out);
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readIntArray(QList<int>& out) const
{
  if (this->array.isEmpty())
    {
    return false;
    }

  out.clear();
  if (this->array.count() == 1 && this->array[0].isEmpty())
    {
    return true;
    }

  foreach (auto const& v, this->array)
    {
    int e;
    if (!v.readInt(e))
      {
      return false;
      }
    out.append(e);
    }
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readLongArray(QList<qint64>& out) const
{
  if (this->array.isEmpty())
    {
    return false;
    }

  out.clear();
  if (this->array.count() == 1 && this->array[0].isEmpty())
    {
    return true;
    }

  foreach (auto const& v, this->array)
    {
    qint64 e;
    if (!v.readLong(e))
      {
      return false;
      }
    out.append(e);
    }
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::Value::readRealArray(QList<double>& out) const
{
  if (this->array.isEmpty())
    {
    return false;
    }

  out.clear();
  if (this->array.count() == 1 && this->array[0].isEmpty())
    {
    return true;
    }

  foreach (auto const& v, this->array)
    {
    double e;
    if (!v.readReal(e))
      {
      return false;
      }
    out.append(e);
    }
  return true;
}

//END qtKstReader value

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtKstReader reader

//-----------------------------------------------------------------------------
class qtKstReaderPrivate
{
public:

  explicit qtKstReaderPrivate(const QUrl& url,
                              const QRegExp& separator,
                              const QRegExp& terminator);
  explicit qtKstReaderPrivate(const QString& data,
                              const QRegExp& separator,
                              const QRegExp& terminator);
  explicit qtKstReaderPrivate(const qtKstReader::Record& record);
  explicit qtKstReaderPrivate(const QList<qtKstReader::Record>& records);

  bool valid_;
  int record_;
  int value_;

  QList<qtKstReader::Record> records_;

protected:
  void init(const QString& data,
            const QRegExp& separator, const QRegExp& terminator);
  bool readRecord(const QString& data, int& pos, qtKstReader::Record& record,
                  qtKstSeparator separator, qtKstSeparator terminator) const;
  bool readString(const QString& data, int& pos, QString& value) const;
  bool readComment(const QString& data, int& pos) const;
};

//-----------------------------------------------------------------------------
qtKstReaderPrivate::qtKstReaderPrivate(
  const QUrl& url, const QRegExp& separator, const QRegExp& terminator)
  : valid_(false), record_(0), value_(0)
{
  QFile file(url.toLocalFile());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    return;
    }

  init(QString::fromUtf8(file.readAll()), separator, terminator);
}

//-----------------------------------------------------------------------------
qtKstReaderPrivate::qtKstReaderPrivate(
  const QString& data, const QRegExp& separator, const QRegExp& terminator)
  : valid_(false), record_(0), value_(0)
{
  init(data, separator, terminator);
}

//-----------------------------------------------------------------------------
qtKstReaderPrivate::qtKstReaderPrivate(const qtKstReader::Record& record)
  : valid_(true), record_(0), value_(0)
{
  this->records_.append(record);
}

//-----------------------------------------------------------------------------
qtKstReaderPrivate::qtKstReaderPrivate(
  const QList<qtKstReader::Record>& records)
  : valid_(true), record_(0), value_(0), records_(records)
{
}

//-----------------------------------------------------------------------------
void qtKstReaderPrivate::init(
  const QString& data, const QRegExp& separator, const QRegExp& terminator)
{
  if (data.isEmpty())
    {
    return;
    }

  int pos = 0;
  while (pos < data.length())
    {
    qtKstReader::Record record;
    if (!this->readRecord(data, pos, record, separator, terminator))
      {
      return;
      }
    if (record.count())
      {
      this->records_.append(record);
      }
    }
  this->valid_ = !this->records_.isEmpty();
}

//-----------------------------------------------------------------------------
bool qtKstReaderPrivate::readRecord(
  const QString& data, int& pos, qtKstReader::Record& record,
  qtKstSeparator separator, qtKstSeparator terminator) const
{
  bool acceptString = true;
  bool acceptValue = true;
  bool acceptArray = true;

  qtKstReader::Value value;
  while (pos < data.length())
    {
    // Check for end of record
    if (terminator.matches(data, pos))
      {
      pos += terminator.matchedLength();
      record.append(value);
      return true;
      }

    // Check for end of value
    if (separator.matches(data, pos))
      {
      pos += separator.matchedLength();
      record.append(value);
      value = qtKstReader::Value();
      acceptString = true;
      acceptValue = true;
      acceptArray = true;
      continue;
      }

    // Take character
    QChar c = data[pos++];
    if (c.isSpace())
      {
      acceptString = true;
      continue;
      }

    // Check for comment
    if (c == '#')
      {
      if (!this->readComment(data, pos))
        {
        return false;
        }
      continue;
      }

    // Check for array
    if (acceptArray && c == '[')
      {
      const QRegExp arrayTerminator("]", Qt::CaseSensitive,
                                    QRegExp::FixedString);
      if (!this->readRecord(data, pos, value.array,
                            separator, arrayTerminator))
        {
        return false;
        }
      if (value.array.isEmpty())
        {
        qDebug() << "KST parse error: end of file encountered"
                    " while looking for record terminator" << QChar(']');
        return false;
        }
      acceptValue = false;
      acceptArray = false;
      continue;
      }

    // Check for value after array
    if (!acceptValue)
      {
      qDebug() << "KST parse error at" << pos
               << ": value not expected at this time";
      return false;
      }

    // Check for string
    if (acceptString && c == '"')
      {
      if (!this->readString(data, pos, value.value))
        {
        return false;
        }
      acceptArray = false;
      continue;
      }

    // Value
    acceptString = false;
    acceptArray = false;
    value.value += c;
    }

  // A file may end with a comment or extra whitespace
  if (acceptValue && record.isEmpty() &&
      value.value.isEmpty() && this->records_.count())
    {
    return true;
    }

  qDebug() << "KST parse error: end of file encountered"
              " while looking for record terminator"
           << terminator.pattern();
  return false;
}

//-----------------------------------------------------------------------------
bool qtKstReaderPrivate::readString(const QString& data, int& pos,
                                    QString& value) const
{
  while (pos < data.length())
    {
    // Take character
    QChar c = data[pos++];

    // Check for escape
    if (c == '\\')
      {
      if (pos + 1 >= data.length())
        {
        qDebug() << "KST parse error: escape character occurred at end of file";
        return false;
        }
      c = data[pos++];
      // Treat escaped newline as line continuation
      if (c == '\n')
        {
        continue;
        }
      }
    // Check for end of string
    else if (c == '"')
      {
      return true;
      }
    value += c;
    }

  qDebug() << "KST parse error: end of file encountered"
              " while looking for string terminator" << QChar('"');
  return false;
}

//-----------------------------------------------------------------------------
bool qtKstReaderPrivate::readComment(const QString& data, int& pos) const
{
  while (pos < data.length())
    {
    // Take character
    QChar c = data[pos++];

    // Check for end of line
    if (c == '\n')
      {
      return true;
      }
    }

  qDebug() << "KST parse error: end of file encountered"
              " while looking end of line following comment";
  return false;
}

//END qtKstReader reader

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtKstReader API

//-----------------------------------------------------------------------------
QRegExp qtKstReader::defaultSeparator()
{
  return QRegExp{",", Qt::CaseSensitive, QRegExp::FixedString};
}

//-----------------------------------------------------------------------------
QRegExp qtKstReader::defaultTerminator()
{
  return QRegExp{";", Qt::CaseSensitive, QRegExp::FixedString};
}

//-----------------------------------------------------------------------------
qtKstReader::qtKstReader() : d_ptr(0)
{
}

//-----------------------------------------------------------------------------
qtKstReader::qtKstReader(
  const QUrl& url, const QRegExp& separator, const QRegExp& terminator)
  : d_ptr(new qtKstReaderPrivate(url, separator, terminator))
{
}

//-----------------------------------------------------------------------------
qtKstReader::qtKstReader(
  const QString& data, const QRegExp& separator, const QRegExp& terminator)
  : d_ptr(new qtKstReaderPrivate(data, separator, terminator))
{
}

//-----------------------------------------------------------------------------
qtKstReader::~qtKstReader()
{
}

//-----------------------------------------------------------------------------
bool qtKstReader::isValid() const
{
  QTE_D_CONST(qtKstReader);
  return d && d->valid_;
}

//-----------------------------------------------------------------------------
#define TEST_RECORD_POINTER() do { \
  if (record < 0) \
    record = this->currentRecord(); \
  if (this->isEndOfFile(record)) \
    return false; \
  } while(0)

#define TEST_VALUE_POINTER() do { \
  if(value < 0) \
    value = this->currentValue(); \
  if(this->isEndOfRecord(value, record)) \
    return false; \
  } while(0)

#define TEST_POINTER() do { \
  TEST_RECORD_POINTER(); \
  TEST_VALUE_POINTER(); \
  } while(0)

//-----------------------------------------------------------------------------
int qtKstReader::recordCount() const
{
  if (!this->isValid())
    {
    return -1;
    }

  QTE_D_CONST(qtKstReader);
  return d->records_.count();
}

//-----------------------------------------------------------------------------
int qtKstReader::valueCount(int record) const
{
  if (this->isEndOfFile(record))
    {
    return -1;
    }

  QTE_D_CONST(qtKstReader);

  if (record < 0)
    {
    record = d->record_;
    }

  return d->records_[record].count();
}

//-----------------------------------------------------------------------------
int qtKstReader::currentRecord() const
{
  if (!this->isValid())
    {
    return -1;
    }

  QTE_D_CONST(qtKstReader);
  return d->record_;
}

//-----------------------------------------------------------------------------
int qtKstReader::currentValue() const
{
  if (!this->isValid())
    {
    return -1;
    }

  QTE_D_CONST(qtKstReader);
  return d->value_;
}

//-----------------------------------------------------------------------------
bool qtKstReader::nextValue()
{
  if (isEndOfRecord())
    {
    return false;
    }

  QTE_D(qtKstReader);
  d->value_++;
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::nextRecord()
{
  if (isEndOfFile())
    {
    return false;
    }

  QTE_D(qtKstReader);
  d->record_++;
  d->value_ = 0;
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::seek(int record, int value)
{
  if (record < 0 || value < 0)
    {
    return false;
    }
  TEST_POINTER();

  QTE_D(qtKstReader);
  d->record_ = record;
  d->value_ = value;
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::isEndOfRecord(int value, int record) const
{
  if (value < 0)
    {
    value = this->currentValue();
    }
  return (value >= this->valueCount(record));
}

//-----------------------------------------------------------------------------
bool qtKstReader::isEndOfFile(int record) const
{
  if (record < 0)
    {
    record = this->currentRecord();
    }
  return (record >= this->recordCount());
}

//-----------------------------------------------------------------------------
bool qtKstReader::isValueEmpty(int value, int record) const
{
  if (this->isEndOfRecord(value, record))
    {
    return true;
    }

  TEST_POINTER();

  QTE_D_CONST(qtKstReader);
  return d->records_[record][value].isEmpty();
}

//-----------------------------------------------------------------------------
bool qtKstReader::isArrayEmpty(int value, int record) const
{
  if (this->isValueEmpty(value, record))
    {
    return true;
    }

  TEST_POINTER();

  QTE_D_CONST(qtKstReader);
  const Record& r = d->records_[record][value].array;

  if (r.count() == 0)
    {
    return d->records_[record][value].value.isEmpty();
    }

  if (r.count() > 1)
    {
    return false;
    }

  return r[0].isEmpty();
}

//-----------------------------------------------------------------------------
#define READ_TYPE(_name, _type) \
  bool qtKstReader::read##_name(_type& out, int value, int record) const \
    { \
    TEST_POINTER(); \
    QTE_D_CONST(qtKstReader); \
    return d->records_[record][value].read##_name(out); \
    }
#define READ_TYPE_ARRAY(_name, _type) \
  READ_TYPE(_name##Array, QList<_type>)

READ_TYPE(String, QString)
READ_TYPE(Int,    int)
READ_TYPE(Long,   qint64)
READ_TYPE(Real,   double)
READ_TYPE_ARRAY(Int,  int)
READ_TYPE_ARRAY(Long, qint64)
READ_TYPE_ARRAY(Real, double)

//-----------------------------------------------------------------------------
bool qtKstReader::readArray(qtKstReader& out, int value, int record) const
{
  QTE_D_CONST(qtKstReader);

  if (out.d_ptr)
    {
    return false;
    }

  TEST_POINTER();

  const qtKstReader::Value& v = d->records_[record][value];
  if (v.array.isEmpty())
    {
    return false;
    }

  out.d_ptr.reset(new qtKstReaderPrivate(v.array));
  return true;
}

//-----------------------------------------------------------------------------
bool qtKstReader::readTable(qtKstReader& out, int value, int record) const
{
  if (out.d_ptr)
    {
    return false;
    }

  TEST_POINTER();

  QTE_D_CONST(qtKstReader);

  const qtKstReader::Value& v = d->records_[record][value];
  if (v.array.isEmpty())
    {
    return false;
    }

  QList<qtKstReader::Record> records;
  foreach (auto const& r, v.array)
    {
    if (r.array.isEmpty())
      {
      return false;
      }
    records.append(r.array);
    }

  out.d_ptr.reset(new qtKstReaderPrivate(records));
  return true;
}

//END qtKstReader API
