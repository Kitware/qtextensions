// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtStatusManager_h
#define __qtStatusManager_h

#include <QObject>
#include <QString>

#include "../core/qtDebugArea.h"

#include "qtStatusSource.h"

class QLabel;
class QProgressBar;

class qtStatusManagerPrivate;

class QTE_EXPORT qtStatusManager : public QObject
{
  Q_OBJECT

public:
  explicit qtStatusManager(QObject* parent = 0);
  ~qtStatusManager();

  void addStatusLabel(QLabel*);
  void addProgressBar(QProgressBar*);

  void removeStatusLabel(QLabel*);
  void removeProgressBar(QProgressBar*);

  void setDebugArea(qtDebugAreaAccessor area);

  void transferOwnership(const QObject* from, qtStatusSource to);

public slots:
  void setStatusText(qtStatusSource source, QString text = {});
  void setProgress(qtStatusSource source, bool available = false,
                   qreal value = -1);
  void setProgress(qtStatusSource source, bool available, int value,
                   int steps, QString format = "%v / %m");
  void setProgress(qtStatusSource source, bool available, int value,
                   int minimum, int maximum, QString format = "%p%");

protected slots:
  void removeObject(QObject*);
  void removeSource(qtStatusSource);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtStatusManager)

private:
  QTE_DECLARE_PRIVATE(qtStatusManager)
  QTE_DISABLE_COPY(qtStatusManager)
};

#endif
