/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtGradientWidget_h
#define __qtGradientWidget_h

#include <QWidget>

#include "../core/qtGlobal.h"

#include "../util/qtGradient.h"

class QPaintEvent;

class qtGradientWidgetPrivate;

class QTE_EXPORT qtGradientWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
  qtGradientWidget(QWidget* parent = 0);
  virtual ~qtGradientWidget();

  Qt::Orientation orientation() const;
  void setOrientation(Qt::Orientation);

  qtGradient gradient() const;

  virtual QSize minimumSizeHint() const;
  virtual QSize sizeHint() const;

public slots:
  void setGradient(qtGradient);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtGradientWidget)

  virtual void paintEvent(QPaintEvent*);

private:
  QTE_DECLARE_PRIVATE(qtGradientWidget)
  Q_DISABLE_COPY(qtGradientWidget)
};

#endif
