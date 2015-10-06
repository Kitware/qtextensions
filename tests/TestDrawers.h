/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __TestDrawers_h
#define __TestDrawers_h

#include "../core/qtGlobal.h"

#include <QWidget>
#include <QSignalMapper>

class qtDrawer;
class qtDrawerWidget;

class TestDrawersWidget : public QWidget
{
  Q_OBJECT

public:
  TestDrawersWidget(QWidget* parent = 0);
  virtual ~TestDrawersWidget();

public slots:
  void addDrawerBefore(QWidget*);
  void addDrawerChild(QWidget*);
  void removeDrawer(QWidget*);
  void addRootDrawer();
  void clearDrawers();

protected:
  void setupDrawer(qtDrawer*);

  QSignalMapper mapAddBefore, mapAddChild, mapRemove;
  qtDrawerWidget* drawerWidget;

private:
  QTE_DISABLE_COPY(TestDrawersWidget)
};

#endif
