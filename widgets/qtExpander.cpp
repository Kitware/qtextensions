/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QApplication>
#include <QStyle>
#include <QStylePainter>
#include <QStyleOptionToolButton>

#include "qtExpander.h"

//-----------------------------------------------------------------------------
qtExpander::qtExpander(QWidget* parent)
  : QToolButton(parent)
{
  this->init();
}

//-----------------------------------------------------------------------------
qtExpander::qtExpander(bool initiallyExpanded, QWidget* parent)
  : QToolButton(parent)
{
  this->init();
  this->setChecked(initiallyExpanded);
}

//-----------------------------------------------------------------------------
void qtExpander::init()
{
  this->setAutoRaise(true);
  this->setCheckable(true);
}

//-----------------------------------------------------------------------------
QSize qtExpander::sizeHint() const
{
  return qtExpander::size(this->style(), this->text(), this->font());
}

//-----------------------------------------------------------------------------
QSize qtExpander::minimumSizeHint() const
{
  return qtExpander::size(this->style(), this->text(), this->font());
}

//-----------------------------------------------------------------------------
QSize qtExpander::arrowSize()
{
  // Ideally, we'd use the actual size of the indicator arrow plus some
  // padding, but that doesn't seem to be available. The indicator (checkbox)
  // size seems like it would be a reasonable choice, but it is too big with
  // Oxygen and too small with many of Qt's built-in styles. So just use 15 and
  // hope for the best.
  //int aw = style->pixelMetric(QStyle::PM_IndicatorWidth);
  //int ah = style->pixelMetric(QStyle::PM_IndicatorHeight);
  return {15, 15};
}

//-----------------------------------------------------------------------------
int qtExpander::margin(QStyle* style, bool hasText)
{
  QStyle::PixelMetric pm =
    (hasText ? QStyle::PM_ButtonMargin : QStyle::PM_ToolBarItemMargin);
  return style->pixelMetric(pm);
}

//-----------------------------------------------------------------------------
QSize qtExpander::size(QStyle* style, QString text, QFont font)
{
  style || (style = QApplication::style());

  int m = qtExpander::margin(style, !text.isEmpty());
  QSize size = qtExpander::arrowSize() + QSize(2 * m, 2 * m);
  if (!text.isEmpty())
    {
    QFontMetrics fm(font);
    QSize textSize = fm.size(Qt::TextShowMnemonic, text);
    textSize += QSize(size.width() + m, 2 * m);
    size = size.expandedTo(textSize);
    }
  return size;
}

//-----------------------------------------------------------------------------
void qtExpander::paintEvent(QPaintEvent*)
{
  // Initialize drawing options
  QStylePainter p(this);
  QStyleOptionToolButton opt;
  this->initStyleOption(&opt);
  opt.icon = QIcon();

  // Adjust state to compensate for overloading of 'checked' to mean 'expanded'
  QStyle::State raiseFlags = QStyle::State_AutoRaise | QStyle::State_MouseOver;
  if ((opt.state & raiseFlags) == raiseFlags)
    {
    opt.state |= QStyle::State_Raised;
    }
  opt.state &= ~(QStyle::State_On);

  // Hand off to the style to draw the widget base
  QString text = opt.text;
  opt.text.clear();
  p.drawComplexControl(QStyle::CC_ToolButton, opt);

  // Get margin
  int m = qtExpander::margin(this->style(), !text.isEmpty());

  // Draw text
  if (!text.isEmpty())
    {
    // Get arrow size
    int aw = qtExpander::arrowSize().width();

    // Draw text
    p.drawItemText(opt.rect.adjusted(aw, 0, 0, 0), Qt::AlignCenter,
                   this->palette(), this->isEnabledToTLW(), text,
                   QPalette::ButtonText);

    // Shrink rect to just cover arrow
    opt.rect.setWidth(aw + (2 * m));
    }

  // Remove margins and draw arrow
  opt.rect.adjust(m, m, -m, -m);
  if (this->isChecked())
    {
    p.drawPrimitive(QStyle::PE_IndicatorArrowDown, opt);
    }
  else
    {
    p.drawPrimitive(QStyle::PE_IndicatorArrowRight, opt);
    }
}
