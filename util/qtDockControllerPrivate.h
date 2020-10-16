// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
