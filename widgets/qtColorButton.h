/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtColorButton_h
#define __qtColorButton_h

#include <QPushButton>

#include "../core/qtGlobal.h"

class qtColorButtonPrivate;

class QTE_EXPORT qtColorButton : public QPushButton
{
  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor)
  Q_PROPERTY(bool alphaShown READ isAlphaShown WRITE setAlphaShown)

public:
  qtColorButton(QWidget* parent = 0);
  virtual ~qtColorButton();

  virtual QSize minimumSizeHint() const QTE_OVERRIDE;
  virtual QSize sizeHint() const QTE_OVERRIDE;

  virtual QColor color() const;
  virtual void setColor(QColor);

  virtual bool isAlphaShown() const;

signals:
  void colorChanged(QColor);

public slots:
  virtual void pickColor();
  virtual void setAlphaShown(bool);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtColorButton)

  void paintEvent(QPaintEvent*);

private:
  QTE_DECLARE_PRIVATE(qtColorButton)
  Q_DISABLE_COPY(qtColorButton)
};

#endif
