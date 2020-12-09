// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  qtColorButton(QWidget* parent = nullptr);
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
  QTE_DISABLE_COPY(qtColorButton)
};

#endif
