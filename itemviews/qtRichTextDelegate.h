// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtRichTextDelegate_h
#define __qtRichTextDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class QTE_EXPORT qtRichTextDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtRichTextDelegate(QObject* parent = nullptr);
  virtual ~qtRichTextDelegate();

protected:
  virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
                     const QModelIndex& index) const QTE_OVERRIDE;
  virtual QSize sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const QTE_OVERRIDE;

private:
  QTE_DISABLE_COPY(qtRichTextDelegate);
};

#endif
