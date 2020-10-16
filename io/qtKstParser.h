// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QString>

namespace qtKstParser
{
  bool parseLong(const QString& str, qint64& out);
  bool parseReal(const QString& str, double& out);
}
