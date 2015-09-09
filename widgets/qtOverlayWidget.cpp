/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
