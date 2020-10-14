// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtGradientEditor.h"
#include "ui_qtGradientEditor.h"

#include "../itemviews/qtColorButtonItemWidget.h"
#include "../itemviews/qtDoubleSpinBoxDelegate.h"

#include "../util/qtColorUtil.h"
#include "../util/qtGradient.h"

#include "../core/qtEnumerate.h"
#include "../core/qtUtil.h"

QTE_IMPLEMENT_D_FUNC(qtGradientEditor)

namespace // anonymous
{

typedef QPair<QTreeWidgetItem*, QTreeWidgetItem*> QTreeWidgetItemPair;

//-----------------------------------------------------------------------------
int minBit(int value)
{
    if (!value)
      return -1;

    int result = 0;
    while (!(value & (1 << result)))
        ++result;

    return result;
}

//-----------------------------------------------------------------------------
QColor::Spec blendSpace(qtGradient::InterpolationMode interpolateMode)
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

} // namespace <anonymous>

//-----------------------------------------------------------------------------
class qtGradientEditorPrivate
{
public:
    void addStop(qtGradient::Stop const&);
    void addStopItem(qtGradient::Stop const&);

    QTreeWidgetItemPair tweeningStops() const;

    Ui::qtGradientEditor ui;

    qtGradient gradient;
};

//-----------------------------------------------------------------------------
void qtGradientEditorPrivate::addStop(qtGradient::Stop const& stop)
{
    if (gradient.insertStop(stop))
    {
        addStopItem(stop);
        ui.gradient->setGradient(gradient);
    }
}

//-----------------------------------------------------------------------------
void qtGradientEditorPrivate::addStopItem(qtGradient::Stop const& stop)
{
    auto const item = new QTreeWidgetItem{ui.stops};

    item->setData(0, Qt::EditRole, stop.color);
    item->setData(1, Qt::EditRole, stop.position);
    item->setData(2, Qt::EditRole, stop.weight);

    item->setFlags(item->flags() | Qt::ItemIsEditable);

    ui.stops->sortByColumn(1, Qt::AscendingOrder);

    new qtColorButtonItemWidget(item, 0);
}

//-----------------------------------------------------------------------------
QTreeWidgetItemPair qtGradientEditorPrivate::tweeningStops() const
{
    auto const cur = ui.stops->currentItem();

    if (!cur)
        return {ui.stops->topLevelItem(0), ui.stops->topLevelItem(1)};

    auto const i = ui.stops->indexOfTopLevelItem(cur);
    auto const next = ui.stops->topLevelItem(i + 1);

    if (next)
        return {cur, next};

    auto const prev = ui.stops->topLevelItem(i - 1);
    return {prev, cur};
}

//-----------------------------------------------------------------------------
qtGradientEditor::qtGradientEditor(QWidget* parent, Qt::WindowFlags flags)
    : QDialog{parent, flags}, d_ptr{new qtGradientEditorPrivate}
{
    QTE_D();

    d->ui.setupUi(this);
    qtUtil::setStandardIcons(d->ui.buttonBox);

    auto const positionDelegate = new qtDoubleSpinBoxDelegate{this};
    positionDelegate->setRange(0.0, 1.0);
    positionDelegate->setPrecision(2);

    d->ui.stops->setItemDelegateForColumn(1, positionDelegate);
    d->ui.stops->setItemDelegateForColumn(2, positionDelegate);

    connect(d->ui.addStop, SIGNAL(clicked(bool)),
            this, SLOT(addStop()));
    connect(d->ui.removeStops, SIGNAL(clicked(bool)),
            this, SLOT(removeStops()));
    connect(d->ui.mode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateInterpolationMode()));
    connect(d->ui.colorSpace, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateInterpolationMode()));
    connect(d->ui.stops, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
            this, SLOT(updateStops()));

    setGradient({});

    qtUtil::resizeColumnsToContents(d->ui.stops);
}

//-----------------------------------------------------------------------------
qtGradientEditor::~qtGradientEditor()
{
}

//-----------------------------------------------------------------------------
qtGradient qtGradientEditor::gradient() const
{
    QTE_D();
    return d->gradient;
}

//-----------------------------------------------------------------------------
void qtGradientEditor::setGradient(qtGradient const& gradient)
{
    QTE_D();

    d->ui.stops->clear();
    foreach (auto const& stop, gradient.stops())
        d->addStopItem(stop);

    static auto const ifm = qtGradient::InterpolateFunctionMask;
    static auto const csm = qtGradient::InterpolateColorspaceMask;
    static auto const ifs = minBit(ifm);
    static auto const css = minBit(csm);

    auto const im = static_cast<int>(gradient.interpolationMode());
    d->ui.mode->setCurrentIndex((im & ifm) >> ifs);
    d->ui.colorSpace->setCurrentIndex((im & csm) >> css);

    d->ui.gradient->setGradient(gradient);
    d->gradient = gradient;
}

//-----------------------------------------------------------------------------
void qtGradientEditor::addStop()
{
    QTE_D();

    auto const count = d->ui.stops->topLevelItemCount();
    if (count < 1)
    {
        d->addStop({1.0, Qt::black});
    }
    else if (count < 2)
    {
        auto const item = d->ui.stops->topLevelItem(0);
        auto const color = item->data(0, Qt::EditRole).value<QColor>();
        auto const pos = item->data(1, Qt::EditRole).toReal();

        d->addStop({(pos < 0.5 ? 1.0 : 0.0), color});
    }
    else
    {
        auto const cs = blendSpace(d->gradient.interpolationMode());

        auto const ts = d->tweeningStops();

        auto const color1 = ts.first->data(0, Qt::EditRole).value<QColor>();
        auto const color2 = ts.second->data(0, Qt::EditRole).value<QColor>();
        auto const pos1 = ts.first->data(1, Qt::EditRole).toReal();
        auto const pos2 = ts.second->data(1, Qt::EditRole).toReal();

        auto const color = qtColorUtil::blend(color1, color2, 0.5, cs);
        auto const pos = 0.5 * (pos1 + pos2);

        d->addStop({pos, color});
    }
}

//-----------------------------------------------------------------------------
void qtGradientEditor::removeStops()
{
    QTE_D();
    qDeleteAll(d->ui.stops->selectedItems());
    updateStops();
}

//-----------------------------------------------------------------------------
void qtGradientEditor::updateStops()
{
    QTE_D();

    d->ui.stops->sortByColumn(1, Qt::AscendingOrder);

    auto stops = QList<qtGradient::Stop>{};
    for (auto const item : qtChildren(d->ui.stops->invisibleRootItem()))
    {
        auto const& color = item->data(0, Qt::EditRole).value<QColor>();
        auto const pos = item->data(1, Qt::EditRole).toReal();
        auto const weight = item->data(2, Qt::EditRole).toReal();
        stops.append({pos, color, weight});
    }

    d->gradient.setStops(stops);
    d->ui.gradient->setGradient(d->gradient);
}

//-----------------------------------------------------------------------------
void qtGradientEditor::updateInterpolationMode()
{
    QTE_D();

    static auto const ifs = minBit(qtGradient::InterpolateFunctionMask);
    static auto const css = minBit(qtGradient::InterpolateColorspaceMask);

    auto const ifi = d->ui.mode->currentIndex();
    auto const csi = d->ui.colorSpace->currentIndex();

    auto const icf = static_cast<qtGradient::InterpolationFlag>(ifi << ifs);
    auto const csf = static_cast<qtGradient::InterpolationFlag>(csi << css);

    d->gradient.setInterpolationMode(icf | csf);
    d->ui.gradient->setGradient(d->gradient);
}
