/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtColorButton.h"

#include "../util/qtColorUtil.h"

#include "../core/qtIndexRange.h"

#include <QBrush>
#include <QColorDialog>
#include <QImage>
#include <QStyle>
#include <QStyleOptionFrameV3>
#include <QPainter>
#include <QPixmap>

QTE_IMPLEMENT_D_FUNC(qtColorButton)

//-----------------------------------------------------------------------------
class qtColorButtonPrivate
{
public:
  QBrush brush;
  QColor color;
  bool alphaShown;

  QPalette fillPalette(QPalette ref) const;
};

//-----------------------------------------------------------------------------
QPalette qtColorButtonPrivate::fillPalette(QPalette ref) const
{
  foreach (auto const n, qtIndexRange<int>(QPalette::NColorGroups))
    {
    QPalette::ColorGroup cg = static_cast<QPalette::ColorGroup>(n);
    QColor c =
      qtColorUtil::blend(ref.color(cg, QPalette::Button), this->color, 0.5);
    ref.setBrush(cg, QPalette::Base, this->brush);
    ref.setColor(cg, QPalette::Window, c);
    }
  return ref;
}

//-----------------------------------------------------------------------------
qtColorButton::qtColorButton(QWidget* parent)
  : QPushButton(parent), d_ptr(new qtColorButtonPrivate)
{
  QTE_D(qtColorButton);
  connect(this, SIGNAL(clicked()), this, SLOT(pickColor()));

  d->alphaShown = false;
  d->color = Qt::black;
  d->brush = Qt::black;
}

//-----------------------------------------------------------------------------
qtColorButton::~qtColorButton()
{
}

//-----------------------------------------------------------------------------
QSize qtColorButton::minimumSizeHint() const
{
  QSize hint = QPushButton::minimumSizeHint();
  hint.setWidth(qMax(hint.width(), 20));
  return hint;
}

//-----------------------------------------------------------------------------
QSize qtColorButton::sizeHint() const
{
  QSize hint = QPushButton::sizeHint();
  hint.setWidth(qMax(hint.width(), 20));
  return hint;
}

//-----------------------------------------------------------------------------
QColor qtColorButton::color() const
{
  QTE_D_CONST(qtColorButton);
  return d->color;
}

//-----------------------------------------------------------------------------
void qtColorButton::setColor(QColor color)
{
  QTE_D(qtColorButton);
  if (d->color != color)
    {
    d->color = color;
    if (color.alpha() == 255)
      {
      d->brush = color;
      }
    else
      {
      const qreal k = color.alphaF();
      QImage tex(8, 8, QImage::Format_RGB32);
      foreach (auto const i, qtIndexRange(4))
        {
        foreach (auto const j, qtIndexRange(4))
          {
          QColor c1 = this->palette().color(QPalette::Button);
          QColor c2 = this->palette().color(QPalette::ButtonText);
          c2 = qtColorUtil::blend(c1, c2, 0.4);
          c1 = qtColorUtil::blend(c1, color, k);
          c2 = qtColorUtil::blend(c2, color, k);
          tex.setPixel(i + 0, j + 0, c1.rgb());
          tex.setPixel(i + 4, j + 4, c1.rgb());
          tex.setPixel(i + 0, j + 4, c2.rgb());
          tex.setPixel(i + 4, j + 0, c2.rgb());
          }
        }
      d->brush.setTexture(QPixmap::fromImage(tex));
      }
    emit this->colorChanged(color);
    this->update();
    }
}

//-----------------------------------------------------------------------------
bool qtColorButton::isAlphaShown() const
{
  QTE_D_CONST(qtColorButton);
  return d->alphaShown;
}

//-----------------------------------------------------------------------------
void qtColorButton::setAlphaShown(bool shown)
{
  QTE_D(qtColorButton);
  d->alphaShown = shown;
}

//-----------------------------------------------------------------------------
void qtColorButton::pickColor()
{
  QTE_D(qtColorButton);

  QColorDialog dlg;
  dlg.setOption(QColorDialog::ShowAlphaChannel, d->alphaShown);
  dlg.setCurrentColor(d->color);

  if (dlg.exec() == QDialog::Accepted)
    {
    this->setColor(dlg.currentColor());
    }
}

//-----------------------------------------------------------------------------
void qtColorButton::paintEvent(QPaintEvent* e)
{
  QTE_D_CONST(qtColorButton);

  QPushButton::paintEvent(e);

  QPainter painter(this);
  QStyle* s = this->style();
  QStyleOptionFrameV3 opt;

  opt.frameShape = QFrame::StyledPanel;
  opt.state |= QStyle::State_Sunken;
  opt.lineWidth = s->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt);
  opt.palette = d->fillPalette(this->palette());

  int adjust = s->pixelMetric(QStyle::PM_ButtonMargin);
  opt.rect = this->rect().adjusted(adjust, adjust, -adjust, -adjust);

  s->drawPrimitive(QStyle::PE_PanelLineEdit, &opt, &painter);
}
