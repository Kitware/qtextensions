// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDoubleSpinBoxDelegate_h
#define __qtDoubleSpinBoxDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class qtDoubleSpinBoxDelegatePrivate;

class QTE_EXPORT qtDoubleSpinBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtDoubleSpinBoxDelegate(QObject* parent = nullptr);
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
  QTE_DISABLE_COPY(qtDoubleSpinBoxDelegate)
};

#endif
