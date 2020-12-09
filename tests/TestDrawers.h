// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  TestDrawersWidget(QWidget* parent = nullptr);
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
