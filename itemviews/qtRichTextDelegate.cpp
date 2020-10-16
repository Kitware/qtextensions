// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtRichTextDelegate.h"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QPainter>
#include <QTextDocument>

namespace // anonymous
{

//-----------------------------------------------------------------------------
int itemMargin(const QStyleOptionViewItem& opt)
{
  QStyle* const style =
    (opt.widget ? opt.widget->style() : QApplication::style());
  return style->pixelMetric(QStyle::PM_FocusFrameHMargin, &opt, opt.widget);
}

//-----------------------------------------------------------------------------
void buildItemDocument(QTextDocument& doc, const QStyleOptionViewItem& opt)
{
  doc.setHtml(opt.text);
  doc.setDocumentMargin(0);
  doc.setTextWidth(opt.rect.width() - (2 * itemMargin(opt)));
}

} // namespace <anonymous>

//-----------------------------------------------------------------------------
qtRichTextDelegate::qtRichTextDelegate(QObject* parent) :
  QStyledItemDelegate(parent)
{
}

//-----------------------------------------------------------------------------
qtRichTextDelegate::~qtRichTextDelegate()
{
}

//-----------------------------------------------------------------------------
void qtRichTextDelegate::paint(
  QPainter* painter, const QStyleOptionViewItem& option,
  const QModelIndex& index) const
{
  QStyleOptionViewItem opt = option;
  this->initStyleOption(&opt, index);

  QStyle* const style =
    (opt.widget ? opt.widget->style() : QApplication::style());

  // Build HTML document from text
  QTextDocument doc;
  buildItemDocument(doc, opt);

  // Paint item without text
  opt.text.clear();
  style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

  QAbstractTextDocumentLayout::PaintContext ctx;

  // Use selected text color if item is selected
  if (opt.state & QStyle::State_Selected)
    {
    const QColor selectedTextColor =
      opt.palette.color(QPalette::Active, QPalette::HighlightedText);
    ctx.palette.setColor(QPalette::Text, selectedTextColor);
    }

  // Render text
  const QRect textRect =
    style->subElementRect(QStyle::SE_ItemViewItemText, &opt);
  painter->save();
  painter->translate(textRect.topLeft());
  painter->setClipRect(textRect.translated(-textRect.topLeft()));
  doc.documentLayout()->draw(painter, ctx);
  painter->restore();
}

//-----------------------------------------------------------------------------
QSize qtRichTextDelegate::sizeHint(
  const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QStyleOptionViewItem opt = option;
  this->initStyleOption(&opt, index);

  // Build HTML document from text
  QTextDocument doc;
  buildItemDocument(doc, opt);

  // Get document size
  auto const documentSize = QSizeF{doc.idealWidth(), doc.size().height()};

  // Return document size adjusted by item margin
  return documentSize.toSize() + QSize{2 * itemMargin(opt), 0};
}
