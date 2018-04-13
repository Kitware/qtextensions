/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtProgressWidget_h
#define qtProgressWidget_h

// QTE includes
#include "../core/qtGlobal.h"

// Qt includes
#include <QPointer>
#include <QVector>
#include <QWidget>

// Forward declarations
class qtProgressWidgetPrivate;

/* Custom widget that shows progress bars and associated labels in a size
 * consistent manner.
 */
class QTE_EXPORT qtProgressWidget : public QWidget
{
  Q_OBJECT

  /// This property controls whether the widget will be hidden automatically
  /// when there are no active progress bars. (Default: true)
  Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide)

  /// This property controls whether the widget should show a busy progress bar
  /// when the its value is 0. This can be used in special cases where the task
  /// being monitored does not report its progress - only that it started and
  /// ended. (Default: false)
  Q_PROPERTY(bool busyOnZero READ busyOnZero WRITE setBusyOnZero)

  typedef QWidget Superclass;

public:
  qtProgressWidget(QWidget* parent = 0);
  virtual ~qtProgressWidget();

  bool autoHide();
  bool busyOnZero();

public slots:
  virtual void updateProgress(int value, QString text);
  void setAutoHide(bool hide);
  void setBusyOnZero(bool busy);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtProgressWidget)

private:
  QTE_DECLARE_PRIVATE(qtProgressWidget)
  QTE_DISABLE_COPY(qtProgressWidget)
};

#endif // __qtProgressWidget_h
