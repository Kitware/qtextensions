/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDockControllerPrivate_h
#define __qtDockControllerPrivate_h

#include <QSignalMapper>
#include <QHash>

#include "qtDockController.h"

class qtDockControllerPrivate : public QObject
{
  Q_OBJECT

public:
  qtDockControllerPrivate();

  QSignalMapper* mapper;
  QHash<QObject*, QAction*> map;

public slots:
  void toggled(QObject*);

private:
  QTE_DISABLE_COPY(qtDockControllerPrivate)
};

#endif
