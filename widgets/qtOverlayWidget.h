// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  QTE_DISABLE_COPY(qtOverlayWidget)
};

#endif
