// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtColorUtil.h"

namespace qtColorUtil
{

//-----------------------------------------------------------------------------
qreal blend(qreal a, qreal b, qreal t)
{
  return (a * (1.0 - t)) + (b * t);
}

//-----------------------------------------------------------------------------
qreal blend(qreal p0, qreal p1, qreal p2, qreal p3, qreal t)
{
  double t2 = t * t;
  double t3 = t2 * t;

  double m0 = (p2 - p0) * 0.5;
  double m1 = (p3 - p1) * 0.5;
  double a0 =  2 * t3  +  -3 * t2  + 1;
  double a1 =  1 * t3  +  -2 * t2  + t;
  double a2 =  1 * t3  +  -1 * t2;
  double a3 = -2 * t3  +   3 * t2;

  return (a0 * p1) + (a1 * m0) + (a2 * m1) + (a3 * p2);
}

//-----------------------------------------------------------------------------
QColor blend(const QColor& a, const QColor& b, qreal t, QColor::Spec s)
{
  QColor result;

  switch (s)
    {
    case QColor::Cmyk:
      result.setCmykF(
        blend(a.cyanF(),    b.cyanF(),    t),
        blend(a.magentaF(), b.magentaF(), t),
        blend(a.yellowF(),  b.yellowF(),  t),
        blend(a.blackF(),   b.blackF(),   t));
      break;
    case QColor::Hsv:
      result.setHsvF(
        blend(a.hsvHueF(),        b.hsvHueF(),        t),
        blend(a.hsvSaturationF(), b.hsvSaturationF(), t),
        blend(a.valueF(),         b.valueF(),         t));
      break;
    case QColor::Hsl:
      result.setHslF(
        blend(a.hslHueF(),        b.hslHueF(),        t),
        blend(a.hslSaturationF(), b.hslSaturationF(), t),
        blend(a.lightnessF(),     b.lightnessF(),     t));
      break;
    default: // RGB
      result.setRgbF(
        blend(a.redF(),   b.redF(),   t),
        blend(a.greenF(), b.greenF(), t),
        blend(a.blueF(),  b.blueF(),  t));
      break;
    }

  // Blend alpha and return result
  result.setAlphaF(blend(a.alphaF(), b.alphaF(), t));
  return result;
}

//-----------------------------------------------------------------------------
#define BLEND4(_c) \
  qBound(0.0, blend(a._c##F(), b._c##F(), c._c##F(), d._c##F(), t), 1.0)
QColor blend(const QColor& a, const QColor& b, const QColor& c,
             const QColor& d, qreal t, QColor::Spec s)
{
  QColor result;

  switch (s)
    {
    case QColor::Cmyk:
      result.setCmykF(
        BLEND4(cyan), BLEND4(magenta), BLEND4(yellow), BLEND4(black));
      break;
    case QColor::Hsv:
      result.setHsvF(
        BLEND4(hsvHue), BLEND4(hsvSaturation), BLEND4(value));
      break;
    case QColor::Hsl:
      result.setHslF(
        BLEND4(hslHue), BLEND4(hslSaturation), BLEND4(lightness));
      break;
    default: // RGB
      result.setRgbF(BLEND4(red), BLEND4(green), BLEND4(blue));
      break;
    }

  // Blend alpha and return result
  result.setAlphaF(BLEND4(alpha));
  return result;
}

} // namespace qtColorUtil
