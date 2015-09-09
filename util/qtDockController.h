/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
  Q_DISABLE_COPY(qtDockController)
};

#endif
