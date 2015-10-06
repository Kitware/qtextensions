/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtSpinBoxDelegate_h
#define __qtSpinBoxDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class qtSpinBoxDelegatePrivate;

class QTE_EXPORT qtSpinBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtSpinBoxDelegate(QObject* parent = 0);
  virtual ~qtSpinBoxDelegate();

  void setDataRole(Qt::ItemDataRole);

  void setRange(int minimum, int maximum);
  void setAlignment(Qt::Alignment);

  virtual QWidget* createEditor(QWidget* parent,
                                const QStyleOptionViewItem&,
                                const QModelIndex&) const;

  virtual void setModelData(QWidget* editor,
                            QAbstractItemModel*,
                            const QModelIndex&) const;

  virtual void setEditorData(QWidget* editor, const QModelIndex&) const;

  virtual void updateEditorGeometry(QWidget* editor,
                                    const QStyleOptionViewItem&,
                                    const QModelIndex&) const;

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtSpinBoxDelegate)

private:
  QTE_DECLARE_PRIVATE(qtSpinBoxDelegate)
  QTE_DISABLE_COPY(qtSpinBoxDelegate)
};

#endif
