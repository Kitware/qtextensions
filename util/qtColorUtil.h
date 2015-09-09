/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
