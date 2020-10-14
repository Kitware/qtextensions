// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  QTE_DISABLE_COPY(qtGradientWidget)
};

#endif
