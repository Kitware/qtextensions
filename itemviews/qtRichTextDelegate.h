/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtRichTextDelegate_h
#define __qtRichTextDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class QTE_EXPORT qtRichTextDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtRichTextDelegate(QObject* parent = 0);
  virtual ~qtRichTextDelegate();

protected:
  virtual void paint(QPainter* painter, const QStyleOptionViewItem& option,
                     const QModelIndex& index) const QTE_OVERRIDE;
  virtual QSize sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const QTE_OVERRIDE;

private:
  Q_DISABLE_COPY(qtRichTextDelegate);
};

#endif
