/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtOverlayWidget_h
#define __qtOverlayWidget_h

#include <QStackedWidget>

#include "../core/qtGlobal.h"

class QTE_EXPORT qtOverlayWidget : public QStackedWidget
{
  Q_OBJECT

public:
  qtOverlayWidget(QWidget* parent = 0);
  virtual ~qtOverlayWidget();

private:
  Q_DISABLE_COPY(qtOverlayWidget)
};

#endif
