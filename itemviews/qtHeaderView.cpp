/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtHeaderView.h"
#include <QMenu>

 //-----------------------------------------------------------------------------
class qtHeaderViewPrivate
{
};

// ----------------------------------------------------------------------------
QTE_IMPLEMENT_D_FUNC(qtHeaderView)

//-----------------------------------------------------------------------------
qtHeaderView::qtHeaderView(Qt::Orientation orientation, QWidget* parent)
  : QHeaderView(orientation, parent),
    d_ptr{ new qtHeaderViewPrivate }
{
  connect(this, &QWidget::customContextMenuRequested,
          this, &qtHeaderView::customContextMenu);
}

//-----------------------------------------------------------------------------
qtHeaderView::~qtHeaderView()
{
}

//-----------------------------------------------------------------------------
void qtHeaderView::customContextMenu(const QPoint &pos)
{
  QTE_D();

  QMenu contextMenu(this);
  for (int i = 0; i <= count(); ++i) {
    const auto text = model()->headerData(i, orientation(),
      Qt::DisplayRole).toString();
    auto action = new QAction;
    action->setText(text);
    action->setCheckable(true);
    action->setChecked(!isSectionHidden(i));
    connect(action, &QAction::toggled,
      this, [this, i](bool show) { setSectionHidden(i, !show); });
  }
  contextMenu.exec(pos);
}

//-----------------------------------------------------------------------------
bool qtHeaderView::restoreState(const QByteArray &state)
{
  // Ensure that the sort indicator state is maintained and sortability is not
  // changed. Workaround for https://bugreports.qt.io/browse/QTBUG-80577

  // Check if sort indicators are shown
  auto const sortIndicatorShown = isSortIndicatorShown();

  auto const restored = QHeaderView::restoreState(state);

  // Reset if sort indicators are shown to its prior value
  setSortIndicatorShown(sortIndicatorShown);

  return restored;
}

//-----------------------------------------------------------------------------
QByteArray qtHeaderView::saveState() const
{
  return QHeaderView::saveState();
}