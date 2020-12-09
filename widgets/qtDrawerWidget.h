// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  qtDrawerWidget(QWidget* parent = nullptr);
  virtual ~qtDrawerWidget();

  virtual qtDrawer* addDrawer(qtDrawer* nextSibling = nullptr);
  virtual void clear();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtDrawerWidget)

  qtDrawer* root();
  virtual qtDrawer* createRoot();

private:
  QTE_DECLARE_PRIVATE(qtDrawerWidget)
  QTE_DISABLE_COPY(qtDrawerWidget)
};

#endif
