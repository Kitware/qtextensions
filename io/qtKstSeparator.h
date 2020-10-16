// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "../core/qtGlobal.h"

class QRegExp;
class QString;

class qtKstSeparatorPrivate;

class qtKstSeparator
{
public:
  qtKstSeparator(const QRegExp& regexp);
  qtKstSeparator(const qtKstSeparator&);
  ~qtKstSeparator();

  bool matches(const QString& data, int pos);
  int matchedLength() const;

  QString pattern() const;

private:
  QTE_DECLARE_PRIVATE_PTR(qtKstSeparator)
  QTE_DECLARE_PRIVATE(qtKstSeparator)

  qtKstSeparator& operator=(const qtKstSeparator&);
};
