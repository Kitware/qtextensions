/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
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
    qtSqueezedLabelPrivate()
    { this->invalidate(nullptr); }

    bool isValid(QString const& text);

    void invalidate(QWidget* self);
    void recalculate(QSize const&, QFontMetrics const&);

    int marginsWidth(QFontMetrics const&) const;
    QRect contentsRect(QRect const&, QFontMetrics const&) const;

    QString elidedText(QString const& text) const;

    static QString ellipsis();

    QString cachedText;
    QString fullText;
    qtSqueezedLabel::ElideMode elideMode;

    bool cacheBackground = false;
    QImage background;

    qreal marginLeft = 0.0;
    qreal marginRight = 0.0;

    int fadeWidth;
    int offset, length;
    bool elided;
};

//-----------------------------------------------------------------------------
bool qtSqueezedLabelPrivate::isValid(QString const& text)
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
            QImage{self->size(), QImage::Format_ARGB32_Premultiplied};
        qtScopedValueChange<bool> cb{this->cacheBackground, true};

        // Calculate our rect relative to our top level widget
        auto* const tlw = self->topLevelWidget();
        auto rect = self->rect();
        rect.setTopLeft(self->mapTo(tlw, rect.topLeft()));
        rect.setBottomRight(self->mapTo(tlw, rect.bottomRight()));

        // Render underlay background
        tlw->render(&this->background, QPoint{}, rect);
    }
}

//-----------------------------------------------------------------------------
void qtSqueezedLabelPrivate::recalculate(
    QSize const& size, QFontMetrics const& fm)
{
    // Check if text fits
    auto const fullWidth = fm.width(this->cachedText);
    auto const availableWidth = size.width() - this->marginsWidth(fm);
    if (fullWidth <= availableWidth)
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
        auto const ellipsisWidth =
            fm.width(qtSqueezedLabelPrivate::ellipsis());
        if (availableWidth <= ellipsisWidth)
        {
            // Nothing at all fits... will only happen if we are smaller than
            // our minimumSizeHint(), but deal with it
            this->length = 0;
            this->offset = 0;
            return;
        }

        // Determine maximum number of characters that can fit
        this->offset = 0;
        this->length = this->cachedText.length();
        auto width = fullWidth + ellipsisWidth;
        while (this->length > 0 && width > availableWidth)
        {
            --this->length;
            width = fm.width(this->cachedText, this->length) + ellipsisWidth;
        }
    }
}

//-----------------------------------------------------------------------------
QString qtSqueezedLabelPrivate::elidedText(QString const& text) const
{
    if (!this->elided)
        return text;

    if (this->elideMode.testFlag(qtSqueezedLabel::ElideFade))
        return text.mid(this->offset, this->length);
    else
        return text.mid(this->offset, this->length) + this->ellipsis();
}

//-----------------------------------------------------------------------------
int qtSqueezedLabelPrivate::marginsWidth(QFontMetrics const& fm) const
{
    auto const margins = this->marginLeft + this->marginRight;
    return static_cast<int>(static_cast<qreal>(fm.height()) * margins);
}

//-----------------------------------------------------------------------------
QRect qtSqueezedLabelPrivate::contentsRect(
    QRect const& rect, QFontMetrics const& fm) const
{
    auto const m = static_cast<qreal>(fm.height());
    auto const ml = static_cast<int>(m * this->marginLeft);
    auto const mr = static_cast<int>(m * this->marginRight);

    return rect.adjusted(ml, 0, -mr, 0);
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
    : QLabel{parent, f}, d_ptr{new qtSqueezedLabelPrivate}
{
    QTE_D();
    d->elideMode = ElideEnd;
}

//-----------------------------------------------------------------------------
qtSqueezedLabel::qtSqueezedLabel(
    QString const& text, QWidget* parent, Qt::WindowFlags f)
    : QLabel{text, parent, f}, d_ptr{new qtSqueezedLabelPrivate}
{
    QTE_D();
    d->elideMode = ElideEnd;
}

//-----------------------------------------------------------------------------
qtSqueezedLabel::~qtSqueezedLabel()
{
}

//-----------------------------------------------------------------------------
qtSqueezedLabel::ElideMode qtSqueezedLabel::elideMode() const
{
    QTE_D();
    return d->elideMode;
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setElideMode(qtSqueezedLabel::ElideMode mode)
{
    QTE_D();
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
    QTE_D();
    return (d->fullText.isNull() ? this->text() : d->fullText);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setFullText(QString const& text)
{
    QTE_D();
    d->fullText = text;
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setText(
    QString const& text, qtSqueezedLabel::SetTextMode mode)
{
    this->setText(mode.testFlag(SimplifyText) ? text.simplified() : text);
    if (mode.testFlag(SetFullText))
        this->setFullText(text);
    if (mode.testFlag(SetToolTip))
        this->setToolTip(text);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::setToolTip(QString const& text, SetToolTipMode mode)
{
    if (mode.testFlag(PlainText))
    {
        auto formattedText = Qt::convertFromPlainText(text);
        if (mode.testFlag(AutoWrap))
        {
            auto const re = QRegExp{QString::fromUtf8("(\xc2\xa0*)\xc2\xa0")};
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
void qtSqueezedLabel::setTextMargins(qreal left, qreal right)
{
    QTE_D();

    left = qMax(0.0, left);
    right = qMax(0.0, right);

    if (d->marginLeft != left || d->marginRight != right)
    {
        d->marginLeft = left;
        d->marginRight = right;
        d->invalidate(this);
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
    QString const& text = this->text();
    if (text.isEmpty())
        return {0, 0};

    QTE_D();

    QFontMetrics const& fm = this->fontMetrics();

    // Calculate minimum width; lesser of full text (in case it is very short)
    // or enough to elide the text
    auto width = fm.width(text);
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
    return {width + d->marginsWidth(fm), fm.height()};
}

//-----------------------------------------------------------------------------
bool qtSqueezedLabel::event(QEvent* e)
{
    if (e->type() == QEvent::WindowActivate ||
        e->type() == QEvent::WindowDeactivate)
    {
        QTE_D();
        d->invalidate(this);
    }

    return QLabel::event(e);
}


//-----------------------------------------------------------------------------
void qtSqueezedLabel::changeEvent(QEvent* e)
{
    if (e->type() == QEvent::FontChange ||
        e->type() == QEvent::EnabledChange ||
        e->type() == QEvent::PaletteChange)
    {
        QTE_D();
        d->invalidate(this);
    }

    QLabel::changeEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::contextMenuEvent(QContextMenuEvent* e)
{
    if (this->contextMenuPolicy() == Qt::DefaultContextMenu)
    {
        auto* const menu = new QMenu(this);
        menu->setAttribute(Qt::WA_DeleteOnClose);

        auto* const action = menu->addAction("&Copy Full Text");
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
    QTE_D();

    // Parent render calls move; don't get stuck in a recursion loop!
    if (!d->cacheBackground)
        d->invalidate(this);

    QLabel::moveEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::resizeEvent(QResizeEvent* e)
{
    QTE_D();

    // Parent render calls resize; don't get stuck in a recursion loop!
    if (!d->cacheBackground)
        d->invalidate(this);

    QLabel::resizeEvent(e);
}

//-----------------------------------------------------------------------------
void qtSqueezedLabel::paintEvent(QPaintEvent* e)
{
    QTE_D();

    // Do nothing if drawing background for cache
    if (d->cacheBackground)
        return;

    // If invalid, or text has changed, recalculate portion to elide
    QString const& text = this->text();
    if (!d->isValid(text))
    {
        d->cachedText = text;
        d->recalculate(this->size(), this->fontMetrics());
    }

    e->accept();

    // Prepare to paint text
    QStyle* style = this->style();
    QPainter painter(this);

    Qt::Alignment const align =
        QStyle::visualAlignment(this->layoutDirection(), this->alignment());
    QPalette::ColorRole const role = this->foregroundRole();

    QStyleOption opt;
    opt.initFrom(this);

    // Paint text
    auto const& elidedText = d->elidedText(text);
    auto const& rect = d->contentsRect(this->rect(), this->fontMetrics());
    if (d->elided && d->elideMode.testFlag(ElideFade))
    {
        auto const width = rect.width();

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
        imagePainter.setCompositionMode(
            QPainter::CompositionMode_DestinationIn);
        imagePainter.fillRect(0, 0, rect.width(), rect.height(), brush);

        // Fade out text by overpainting with blended fade image
        painter.drawImage(rect.topLeft(), fadeImage);
    }
    else
    {
        style->drawItemText(&painter, rect, align, opt.palette,
                            this->isEnabled(), elidedText, role);
    }
}

//END qtSqueezedLabel
