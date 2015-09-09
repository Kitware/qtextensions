/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDrawerWidget_h
#define __qtDrawerWidget_h

#include <QWidget>

#include "../core/qtGlobal.h"

class qtDrawer;
class qtDrawerWidgetPrivate;

class QTE_EXPORT qtDrawerWidget : public QWidget
{
  Q_OBJECT

public:
  qtDrawerWidget(QWidget* parent = 0);
  virtual ~qtDrawerWidget();

  virtual qtDrawer* addDrawer(qtDrawer* nextSibling = 0);
  virtual void clear();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtDrawerWidget)

  qtDrawer* root();
  virtual qtDrawer* createRoot();

private:
  QTE_DECLARE_PRIVATE(qtDrawerWidget)
  Q_DISABLE_COPY(qtDrawerWidget)
};

#endif
