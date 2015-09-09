/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QString>

namespace qtKstParser
{
  bool parseLong(const QString& str, qint64& out);
  bool parseReal(const QString& str, double& out);
}
