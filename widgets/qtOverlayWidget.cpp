// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QStackedLayout>

#include "qtOverlayWidget.h"

//-----------------------------------------------------------------------------
qtOverlayWidget::qtOverlayWidget(QWidget* parent)
  : QStackedWidget(parent)
{
  QStackedLayout* layout = qobject_cast<QStackedLayout*>(this->layout());
  if (layout)
    {
    layout->setStackingMode(QStackedLayout::StackAll);
    }
}

//-----------------------------------------------------------------------------
qtOverlayWidget::~qtOverlayWidget()
{
}
