// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtSpinBoxDelegate_h
#define __qtSpinBoxDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class qtSpinBoxDelegatePrivate;

class QTE_EXPORT qtSpinBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtSpinBoxDelegate(QObject* parent = nullptr);
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
