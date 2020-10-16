// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtColorUtil_h
#define __qtColorUtil_h

#include <QColor>

#include "../core/qtGlobal.h"

namespace qtColorUtil
{
  QTE_EXPORT qreal blend(qreal a, qreal b, qreal t);
  QTE_EXPORT qreal blend(qreal a, qreal b, qreal c, qreal d, qreal t);
  QTE_EXPORT QColor blend(const QColor& a, const QColor& b,
                          qreal t, QColor::Spec s = QColor::Rgb);
  QTE_EXPORT QColor blend(const QColor& a, const QColor& b,
                          const QColor& c, const QColor& d,
                          qreal t, QColor::Spec s = QColor::Rgb);
}

#endif
