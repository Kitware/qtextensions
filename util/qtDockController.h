// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDockController_h
#define __qtDockController_h

#include <QObject>

#include "../core/qtGlobal.h"

class QAction;
class QDockWidget;

class qtDockControllerPrivate;

class QTE_EXPORT qtDockController : public QObject
{
public:
  qtDockController(QObject* parent);
  virtual ~qtDockController();

  void addToggleAction(QAction*, QDockWidget*);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtDockController)

private:
  QTE_DECLARE_PRIVATE(qtDockController)
  QTE_DISABLE_COPY(qtDockController)
};

#endif
