// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtGradient.h"

#include <QSharedData>

#include "../core/qtIndexRange.h"
#include "../core/qtMath.h"

#include "qtColorUtil.h"

QTE_IMPLEMENT_D_FUNC_SHARED(qtGradient)

//BEGIN qtGradientData

//-----------------------------------------------------------------------------
class qtGradientData : public QSharedData
{
public:
    QMap<qreal, qtGradient::Stop> stops;
    qtGradient::InterpolationMode interpolateMode;
    qtGradient::Spread spread;

    QColor blend(QColor const& a, QColor const& b, qreal t) const;
    QColor linearBlend(QColor const& a, QColor const& b,
                       qreal t, qreal w) const;
    QColor cubicBlend(qtGradient::Stop const& a, qtGradient::Stop const& b,
                      qtGradient::Stop const& c, qtGradient::Stop const& d,
                      qreal t) const;

    QColor::Spec blendSpace() const;
};

//-----------------------------------------------------------------------------
QColor::Spec qtGradientData::blendSpace() const
{
    switch (interpolateMode & qtGradient::InterpolateColorspaceMask)
    {
        case qtGradient::InterpolateCmyk:
            return QColor::Cmyk;
        case qtGradient::InterpolateHsv:
            return QColor::Hsv;
        case qtGradient::InterpolateHsl:
            return QColor::Hsl;
        default: // RGB
            return QColor::Rgb;
    }
}

//-----------------------------------------------------------------------------
QColor qtGradientData::blend(QColor const& a, QColor const& b, qreal t) const
{
    return qtColorUtil::blend(a, b, t, blendSpace());
}

//-----------------------------------------------------------------------------
QColor qtGradientData::linearBlend(
    QColor const& a, QColor const& b, qreal t, qreal w) const
{
    // Apply weighting
    t = (t > w ? qtColorUtil::blend(0.5, 1.0, (t - w) / (1.0 - w))
         : qtColorUtil::blend(0.0, 0.5, t / w));

    // Perform blend
    return blend(a, b, t);
}

//-----------------------------------------------------------------------------
QColor qtGradientData::cubicBlend(
    qtGradient::Stop const& a, qtGradient::Stop const& b,
    qtGradient::Stop const& c, qtGradient::Stop const& d, qreal t) const
{
    // Calculate intermediary stops
    auto const& cb = b.color, cc = c.color;
    auto const& ca = blend(a.color, cb, 0.5);
    auto const& cd = blend(d.color, cc, 0.5);
    auto const& cm = blend(cb, cc, 0.5);
    auto const w = b.weight;
    auto const pb = b.position, pc = c.position;
    auto const pa = qtColorUtil::blend(a.position, pb, a.weight);
    auto const pd = qtColorUtil::blend(d.position, pc, c.weight);
    auto const pm = qtColorUtil::blend(pb, pc, w);

    if (t > w)
    {
        t = (t - w) / (1.0 - w);
        t = qtColorUtil::blend(pb, pm, pc, pd, t);
        t = qBound(0.0, (t - pm) / (pc - pm), 1.0);
        return qtColorUtil::blend(cb, cm, cc, cd, t, blendSpace());
    }
    else
    {
        t /= w;
        t = qtColorUtil::blend(pa, pb, pm, pc, t);
        t = qBound(0.0, (t - pb) / (pm - pb), 1.0);
        return qtColorUtil::blend(ca, cb, cm, cc, t, blendSpace());
    }
}

//END qtGradientData

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtGradient

//-----------------------------------------------------------------------------
qtGradient::qtGradient() : d_ptr(new qtGradientData)
{
    QTE_D_MUTABLE();
    d->interpolateMode = qtGradient::InterpolateLinear |
                         qtGradient::InterpolateRgb;
    d->spread = QGradient::PadSpread;
    insertStop(0.0, Qt::black);
    insertStop(1.0, Qt::white);
}

//-----------------------------------------------------------------------------
qtGradient::qtGradient(QList<Stop> const& stops, InterpolationMode im,
                       QGradient::Spread spread, NormalizeMode nm)
    : d_ptr{new qtGradientData}
{
    QTE_D_MUTABLE();
    d->interpolateMode = im;
    d->spread = spread;
    setStops(stops, nm);
}

//-----------------------------------------------------------------------------
qtGradient::qtGradient(qtGradient const& other) : d_ptr{other.d_ptr}
{
}

//-----------------------------------------------------------------------------
qtGradient::~qtGradient()
{
}

//-----------------------------------------------------------------------------
qtGradient& qtGradient::operator=(qtGradient const& other)
{
    d_ptr = other.d_ptr;
    return *this;
}

//-----------------------------------------------------------------------------
qtGradient::InterpolationMode qtGradient::interpolationMode() const
{
    QTE_D_SHARED();
    return d->interpolateMode;
}

//-----------------------------------------------------------------------------
void qtGradient::setInterpolationMode(qtGradient::InterpolationMode im)
{
    QTE_D_MUTABLE();
    d->interpolateMode = im;
}

//-----------------------------------------------------------------------------
qtGradient::Spread qtGradient::spread() const
{
    QTE_D_SHARED();
    return d->spread;
}

//-----------------------------------------------------------------------------
void qtGradient::setSpread(qtGradient::Spread spread)
{
    QTE_D_MUTABLE();
    d->spread = spread;
}

//-----------------------------------------------------------------------------
QMap<qreal, qtGradient::Stop> qtGradient::stops() const
{
    QTE_D_SHARED();
    return d->stops;
}

//-----------------------------------------------------------------------------
void qtGradient::setStops(QList<qtGradient::Stop> const& stops,
                          qtGradient::NormalizeMode nm)
{
    QTE_D_MUTABLE();

    // Handle empty set
    if (stops.isEmpty())
    {
        d->stops.clear();
        return;
    }

    // Handle set with exactly one stop
    if (stops.count() == 1)
    {
        d->stops.clear();
        d->stops.insert(0.0, stops.first());
        return;
    }

    // Convert to map
    QMap<qreal, qtGradient::Stop> stopsMap;
    foreach (auto stop, stops)
    {
      stop.weight = qBound(0.0, stop.weight, 1.0);
      stopsMap.insert(stop.position, stop);
    }

    // Handle 'regular' stop sets, based on normalization mode
    if (nm == qtGradient::NormalizeStops)
    {
        // Calculate coefficients to normalize stops to [0.0, 1.0]
        auto const offset = stopsMap.constBegin().key();
        auto const scale = 1.0 / ((--stopsMap.constEnd()).key() - offset);

        // Recreate stop set, normalized
        d->stops.clear();
        foreach (auto stop, stops)
        {
            stop.position = (stop.position - offset) * scale;
            d->stops.insert(stop.position, stop);
        }
    }
    else
    {
        QMap<qreal, qtGradient::Stop>::iterator iter;

        // Truncate stops outside of normalized range
        while (!stopsMap.isEmpty() && (iter = stopsMap.begin()).key() < 0.0)
            stopsMap.erase(iter);
        while (!stopsMap.isEmpty() && (iter = stopsMap.end()).key() > 1.0)
            stopsMap.erase(iter);

        // Add stops at 0.0, 1.0 if needed
        if (!stopsMap.isEmpty())
        {
            if ((iter = stopsMap.begin()).key() > 0.0)
                stopsMap.insert(0.0, iter.value());
            if ((iter = stopsMap.end()).key() < 1.0)
                stopsMap.insert(1.0, iter.value());
        }

        // Set stops to modified set
        d->stops = stopsMap;
    }
}

//-----------------------------------------------------------------------------
bool qtGradient::insertStop(qtGradient::Stop stop)
{
    if (0.0 > stop.position || stop.position > 1.0)
        return false;

    QTE_D_MUTABLE();

    stop.weight = qBound(0.0, stop.weight, 1.0);
    d->stops.insert(stop.position, stop);

    return true;
}

//-----------------------------------------------------------------------------
bool qtGradient::removeStop(qreal position)
{
    QTE_D_MUTABLE();
    return (d->stops.remove(position) > 0);
}

//-----------------------------------------------------------------------------
QColor qtGradient::at(qreal pos) const
{
    QTE_D_SHARED();

    if (d->stops.isEmpty())
        return Qt::transparent;
    if (d->stops.count() < 2)
        return d->stops.begin().value().color;

    // Apply spread to get normalized position
    switch (d->spread)
    {
        case QGradient::RepeatSpread:
            pos = fmod(pos, 1.0);
            (pos < 0.0) && (pos = 1.0 + pos);
            break;
        case QGradient::ReflectSpread:
            pos = fabs(fmod(pos, 2.0));
            (pos > 1.0) && (pos = 2.0 - pos);
            break;
        default:
            pos = qBound(0.0, pos, 1.0);
            break;
    }

    // Find next stop
    auto const su = d->stops.lowerBound(pos);

    // Check for exact (or 'close enough') match
    if (qFuzzyCompare(pos, su.key()))
        return su.value().color;

    // Find previous stop and calculate relative position
    auto const sl = su - 1;
    auto const rpos = (pos - sl.key()) / (su.key() - sl.key());

    // Check (again) for exact (or 'close enough') match, this time against the
    // previous stop (in case we are off 'just enough' that lowerBound didn't
    // consider it a match)
    if (qFuzzyCompare(pos, sl.key()))
        return sl.value().color;

    // Calculate blended color
    switch (d->interpolateMode & qtGradient::InterpolateFunctionMask)
    {
        case qtGradient::InterpolateDiscrete:
            return (rpos < sl->weight ? sl->color : su->color);

        case qtGradient::InterpolateCubic:
        {
            auto const sll = (sl == d->stops.begin() ? sl : sl - 1);
            auto const suu = (su + 1 == d->stops.end() ? su : su + 1);
            return d->cubicBlend(*sll, *sl, *su, *suu, rpos);
        }

        default: // qtGradient::InterpolateLinear
            return d->linearBlend(sl->color, su->color, rpos, sl->weight);
    }
}

//-----------------------------------------------------------------------------
QList<QColor> qtGradient::render(int size) const
{
    QList<QColor> out;
    out.reserve(size);

    auto const k = static_cast<qreal>(1.0 / (size - 1));
    foreach (auto const i, qtIndexRange(size))
        out.append(at(static_cast<qreal>(i) * k));

    return out;
}

//END qtGradient
