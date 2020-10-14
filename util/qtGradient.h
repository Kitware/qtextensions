// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtGradient_h
#define __qtGradient_h

#include <QGradient>
#include <QMap>
#include <QSharedDataPointer>

#include "../core/qtGlobal.h"

class qtGradientData;

class QTE_EXPORT qtGradient
{
public:
    struct Stop
    {
        Stop(qreal position = 0.0,
             QColor color = Qt::transparent,
             qreal weight = 0.5)
            : color(color), position(position), weight(weight)
        {}

        QColor color;
        qreal position;
        qreal weight;
    };

    enum InterpolationFlag
    {
        InterpolateDiscrete = 0x0,
        InterpolateLinear = 0x1,
        InterpolateCubic = 0x2,
        InterpolateRgb = 0x00,
        InterpolateCmyk = 0x10,
        InterpolateHsv = 0x20,
        InterpolateHsl = 0x30,
        InterpolateFunctionMask = 0xf,
        InterpolateColorspaceMask = 0xf0
    };
    Q_DECLARE_FLAGS(InterpolationMode, InterpolationFlag)

    typedef QGradient::Spread Spread;

    enum NormalizeMode
    {
        NormalizeStops,
        PadStops
    };

    qtGradient();
    qtGradient(QList<Stop> const& stops,
               InterpolationMode = InterpolateLinear,
               Spread = QGradient::PadSpread,
               NormalizeMode = NormalizeStops);
    qtGradient(const qtGradient&);
    ~qtGradient();

    qtGradient& operator=(const qtGradient&);

    InterpolationMode interpolationMode() const;
    void setInterpolationMode(InterpolationMode);
    Spread spread() const;
    void setSpread(Spread);

    QMap<qreal, Stop> stops() const;
    void setStops(QList<Stop> const&, NormalizeMode = NormalizeStops);
    bool insertStop(Stop stop);
    bool removeStop(qreal position);

    bool insertStop(qreal position, QColor color, qreal weight = 0.5)
    { return insertStop(qtGradient::Stop(position, color, weight)); }

    QColor at(qreal) const;
    QList<QColor> render(int size) const;

protected:
    QTE_DECLARE_SHARED_PTR(qtGradient)

private:
    QTE_DECLARE_SHARED(qtGradient)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(qtGradient::InterpolationMode)

#endif
