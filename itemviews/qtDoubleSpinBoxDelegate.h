/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDoubleSpinBoxDelegate_h
#define __qtDoubleSpinBoxDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class qtDoubleSpinBoxDelegatePrivate;

class QTE_EXPORT qtDoubleSpinBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtDoubleSpinBoxDelegate(QObject* parent = 0);
  virtual ~qtDoubleSpinBoxDelegate();

  void setRange(double minimum, double maximum);
  void setPrecision(int precision);
  void setDataRole(Qt::ItemDataRole);

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
  QTE_DECLARE_PRIVATE_RPTR(qtDoubleSpinBoxDelegate)

private:
  QTE_DECLARE_PRIVATE(qtDoubleSpinBoxDelegate)
  Q_DISABLE_COPY(qtDoubleSpinBoxDelegate)
};

#endif
