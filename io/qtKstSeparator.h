/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
