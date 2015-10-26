/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtSqueezedLabel.h"

#include <QApplication>
#include <QClipboard>
#include <QFontMetrics>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QStyle>
#include <QStyleOption>
#include <QTextDocument>

#include "../core/qtScopedValueChange.h"

QTE_IMPLEMENT_D_FUNC(qtSqueezedLabel)

//BEGIN qtSqueezedLabelPrivate

//-----------------------------------------------------------------------------
class qtSqueezedLabelPrivate
{
public:
  qtSqueezedLabelPrivate() : cacheBackground(false)
    { this->invalidate(0); }

  bool isValid(const QString& text);

  void invalidate(QWidget* self);
  void recalculate(const QSize&, const QFontMetrics&);

  static QString ellipsis();

  QString cachedText;
  QString fullText;
  qtSqueezedLabel::ElideMode elideMode;

  bool cacheBackground;
  QImage background;

  int fadeWidth;
  int offset, length;
  bool elided;
};

//-----------------------------------------------------------------------------
bool qtSqueezedLabelPrivate::isValid(const QString& text)
{
  return (length >= 0 && text == this->cachedText);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabelPrivate::invalidate(QWidget* self)
{
  this->offset = -1;
  this->length = -1;
  this->elided = true;

  if (self)
    {
    // Prepare to repaint our underlay background
    this->background =
      QImage(self->size(), QImage::Format_ARGB32_Premultiplied);
    qtScopedValueChange<bool> cb(this->cacheBackground, true);

    // Calculate our rect relative to our top level widget
    QWidget* tlw = self->topLevelWidget();
    QRect rect = self->rect();
    rect.setTopLeft(self->mapTo(tlw, rect.topLeft()));
    rect.setBottomRight(self->mapTo(tlw, rect.bottomRight()));

    // Render underlay background
    tlw->render(&this->background, QPoint(), rect);
    }
}

//-----------------------------------------------------------------------------
void qtSqueezedLabelPrivate::recalculate(
  const QSize& size, const QFontMetrics& fm)
{
  // Check if text fits
  const int fullWidth = fm.width(this->cachedText);
  if (fullWidth <= size.width())
    {
    this->length = this->cachedText.length();
    this->offset = 0;
    this->elided = false;
    return;
    }

  this->elided = true;

  // How are we eliding?
  if (elideMode.testFlag(qtSqueezedLabel::ElideFade))
    {
    this->fadeWidth = fm.height() * 3;

    // \TODO support modes other then ElideEnd
    const int availableWidth = size.width();

    // Determine number of characters needed to fill label
    this->offset = 0;
    this->length = 0;
    int width = 0;
    while (width < availableWidth)
      {
      ++this->length;
      width = fm.width(this->cachedText, this->length);
      }
    }
  else
    {
    // \TODO support modes other then ElideEnd
    const int availableWidth =
      size.width() - fm.width(qtSqueezedLabelPrivate::ellipsis());
    if (availableWidth < 0)
      {
      // Nothing at all fits... will only happen if we are smaller than our
      // minimumSizeHint(), but deal with it
      this->length = 0;
      this->offset = 0;
      return;
      }

    // Determine maximum number of characters that can fit
    this->offset = 0;
    this->length = this->fullText.length();
    int width = fullWidth;
    while (width > availableWidth)
      {
      --this->length;
      width = fm.width(this->cachedText, this->length);
      }
    }
}

//-----------------------------------------------------------------------------
QString qtSqueezedLabelPrivate::ellipsis()
{
  return QString::fromUtf8("\xe2\x80\xa6");
}

//END qtSqueezedLabelPrivate

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtSqueezedLabel

//-----------------------------------------------------------------------------
qtSqueezedLabel::qtSqueezedLabel(QWidget* parent, Qt::WindowFlags f)
  : QLabel(parent, f), d_ptr(new qtSqueezedLabelPrivate)
{
  QTE_D(qtSqueezedLabel);
  d->elideMode = ElideEnd;
}

//-----------------------------------------------------------------------------
qtSqueezedLabel::qtSqueezedLabel(
  const QString& text, QWidget* parent, Qt::WindowFlags f)
  : QLabel(text, parent, f), d_ptr(new qtSqueezedLabelPrivate)
{
  QTE_D(qtSqueezedLabel);
  d->elideMode = ElideEnd;
}

//-----------------------------------------------------------------------------
qtSqueezedLabel::~qtSqueezedLabel()
{
}

//-----------------------------------------------------------------------------
qtSqueezedLabel::ElideMode qtSqueezedLabel::elideMode() const
{
  QTE_D_CONST(qtSqueezedLabel);
  return d->elideMode;
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setElideMode(qtSqueezedLabel::ElideMode mode)
{
  QTE_D(qtSqueezedLabel);
  if (d->elideMode != mode)
    {
    d->elideMode = mode;
    d->length = -1;
    this->update();
    }
}

//-----------------------------------------------------------------------------
QString qtSqueezedLabel::fullText() const
{
  QTE_D_CONST(qtSqueezedLabel);
  return (d->fullText.isNull() ? this->text() : d->fullText);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setFullText(const QString& text)
{
  QTE_D(qtSqueezedLabel);
  d->fullText = text;
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setText(
  const QString& text, qtSqueezedLabel::SetTextMode mode)
{
  this->setText(mode.testFlag(SimplifyText) ? text.simplified() : text);
  if (mode.testFlag(SetFullText))
    {
    this->setFullText(text);
    }
  if (mode.testFlag(SetToolTip))
    {
    this->setToolTip(text);
    }
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setToolTip(const QString& text, SetToolTipMode mode)
{
  if (mode.testFlag(PlainText))
    {
    QString formattedText = Qt::convertFromPlainText(text);
    if (mode.testFlag(AutoWrap))
      {
      const QRegExp re(QString::fromUtf8("(\xc2\xa0*)\xc2\xa0"));
      formattedText.replace(re, "\\1 ");
      }
    this->setToolTip(formattedText);
    }
  else
    {
    this->setToolTip(text);
    }
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::copy()
{
  qApp->clipboard()->setText(this->fullText());
}

//-----------------------------------------------------------------------------
QSize qtSqueezedLabel::minimumSizeHint() const
{
  const QString& text = this->text();
  if (text.isEmpty())
    {
    return {0, 0};
    }

  QTE_D_CONST(qtSqueezedLabel);

  const QFontMetrics& fm = this->fontMetrics();

  // Calculate minimum width; lesser of full text (in case it is very short) or
  // enough to elide the text
  int width = fm.width(text);
  if (d->elideMode.testFlag(ElideFade))
    {
    // Fade minimum size is 3x height
    width = qMin(width, fm.height() * 3);
    }
  else
    {
    // Ellipsis minimum size is enough to show just the ellipsis
    width = qMin(width, fm.width(qtSqueezedLabelPrivate::ellipsis()));
    }
  return {width, fm.height()};
}

//-----------------------------------------------------------------------------
bool qtSqueezedLabel::event(QEvent* e)
{
  if (e->type() == QEvent::WindowActivate ||
      e->type() == QEvent::WindowDeactivate)
    {
    QTE_D(qtSqueezedLabel);
    d->invalidate(this);
    }

  return QLabel::event(e);
}


//-----------------------------------------------------------------------------
void qtSqueezedLabel::changeEvent(QEvent* e)
{
  if (e->type() == QEvent::FontChange || e->type() == QEvent::EnabledChange)
    {
    QTE_D(qtSqueezedLabel);
    d->invalidate(this);
    }

  QLabel::changeEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::contextMenuEvent(QContextMenuEvent* e)
{
  if (this->contextMenuPolicy() == Qt::DefaultContextMenu)
    {
    QMenu* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    QAction* action = menu->addAction("&Copy Full Text");
    action->setEnabled(!this->fullText().isEmpty());
    connect(action, SIGNAL(triggered()), this, SLOT(copy()));

    menu->popup(e->globalPos());
    e->accept();
    return;
    }

  QLabel::contextMenuEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::moveEvent(QMoveEvent* e)
{
  QTE_D(qtSqueezedLabel);

  // Parent render calls move; don't get stuck in a recursion loop!
  if (!d->cacheBackground)
    {
    d->invalidate(this);
    }

  QLabel::moveEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::resizeEvent(QResizeEvent* e)
{
  QTE_D(qtSqueezedLabel);

  // Parent render calls resize; don't get stuck in a recursion loop!
  if (!d->cacheBackground)
    {
    d->invalidate(this);
    }

  QLabel::resizeEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::paintEvent(QPaintEvent* e)
{
  QTE_D(qtSqueezedLabel);

  // Do nothing if drawing background for cache
  if (d->cacheBackground)
    {
    return;
    }

  // If invalid, or text has changed, recalculate portion to elide
  const QString& text = this->text();
  if (!d->isValid(text))
    {
    d->cachedText = text;
    d->recalculate(this->size(), this->fontMetrics());
    }

  // Check if text fits
  if (!d->elided)
    {
    // If so, let ordinary label handle the painting
    QLabel::paintEvent(e);
    return;
    }

  e->accept();

  // Prepare to paint text
  QStyle* style = this->style();
  QPainter painter(this);

  const Qt::Alignment align =
    QStyle::visualAlignment(this->layoutDirection(), this->alignment());
  const QPalette::ColorRole role = this->foregroundRole();

  QStyleOption opt;
  opt.initFrom(this);

  // Paint text
  if (d->elideMode.testFlag(ElideFade))
    {
    const QRect rect = this->rect();
    const int width = rect.width();
    const QString elidedText = text.mid(d->offset, d->length);

    // Draw text
    style->drawItemText(&painter, rect, align, opt.palette,
                        this->isEnabled(), elidedText, role);

    // Set up fade gradient
    int x1, x2;
    if (align.testFlag(Qt::AlignLeft))
      {
      x1 = qMax(0, width - d->fadeWidth);
      x2 = width;
      }
    else
      {
      x1 = qMin(width, d->fadeWidth);
      x2 = 0;
      }
    QLinearGradient grad(x1, 0, x2, 0);
    grad.setColorAt(0.0, Qt::transparent);
    grad.setColorAt(0.9, Qt::black);

    // Fade out background for overpainting
    QImage fadeImage(d->background);
    QBrush brush(grad);
    QPainter imagePainter(&fadeImage);
    imagePainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    imagePainter.fillRect(0, 0, rect.width(), rect.height(), brush);

    // Fade out text by overpainting with blended fade image
    painter.drawImage(rect.topLeft(), fadeImage);
    }
  else
    {
    const QString elidedText =
      text.mid(d->offset, d->length) + qtSqueezedLabelPrivate::ellipsis();
    style->drawItemText(&painter, this->rect(), align, opt.palette,
                        this->isEnabled(), elidedText, role);
    }
}

//END qtSqueezedLabel
