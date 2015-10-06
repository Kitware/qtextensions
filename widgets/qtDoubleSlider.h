/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDoubleSlider_h
#define __qtDoubleSlider_h

#include <QAbstractSlider>

#include "../core/qtGlobal.h"

class QStyleOptionSlider;

class qtDoubleSliderPrivate;

class QTE_EXPORT qtDoubleSlider : public QWidget
{
  Q_OBJECT

  Q_ENUMS(QuantizeMode)

  Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
  Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
  Q_PROPERTY(double singleStep READ singleStep WRITE setSingleStep)
  Q_PROPERTY(double pageStep READ pageStep WRITE setPageStep)
  Q_PROPERTY(double quantize READ quantize WRITE setQuantize)
  Q_PROPERTY(QuantizeMode quantizeMode READ quantizeMode WRITE setQuantizeMode)
  Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged
                          USER true)

public:
  enum QuantizeMode
    {
    QuantizeRelative,
    QuantizeAbsolute
    };

  explicit qtDoubleSlider(QWidget* parent = 0);
  virtual ~qtDoubleSlider();

  double minimum() const;
  double maximum() const;

  double singleStep() const;
  double pageStep() const;
  double quantize() const;
  QuantizeMode quantizeMode() const;

  double value() const;

  void setRange(double min, double max);
  void setQuantize(double, QuantizeMode);
  void setQuantizeMode(QuantizeMode);

  typedef QAbstractSlider::SliderAction SliderAction;
  void triggerAction(SliderAction action);

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

public slots:
  void setMinimum(double);
  void setMaximum(double);

  void setSingleStep(double);
  void setPageStep(double);
  void setQuantize(double);

  void setValue(double);

signals:
  void valueChanged(double);

  void sliderPressed();
  void sliderMoved(double position);
  void sliderReleased();

  void rangeChanged(double min, double max);

  void actionTriggered(int action);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtDoubleSlider)

  void initStyleOption(QStyleOptionSlider*) const;

  virtual bool event(QEvent*);

  virtual void paintEvent(QPaintEvent*);
  virtual void resizeEvent(QResizeEvent*);
  virtual void keyPressEvent(QKeyEvent*);
  virtual void mousePressEvent(QMouseEvent*);
  virtual void mouseReleaseEvent(QMouseEvent*);
  virtual void mouseMoveEvent(QMouseEvent*);
  virtual void wheelEvent(QWheelEvent*);

private:
  QTE_DECLARE_PRIVATE(qtDoubleSlider)
  QTE_DISABLE_COPY(qtDoubleSlider)
};

#endif
