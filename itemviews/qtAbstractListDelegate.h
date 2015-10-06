/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtAbstractListDelegate_h
#define __qtAbstractListDelegate_h

#include <QStyledItemDelegate>

#include "../core/qtGlobal.h"

class qtAbstractListDelegatePrivate;

class QTE_EXPORT qtAbstractListDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  qtAbstractListDelegate(QObject* parent = 0);
  virtual ~qtAbstractListDelegate();

  virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                                const QModelIndex&) const;

  using QStyledItemDelegate::setModelData;
  virtual void setEditorData(QWidget* editor, const QModelIndex&) const;

protected slots:
  void editorValueChanged();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtAbstractListDelegate)

  virtual QWidget* createListEditor(QWidget* parent) const = 0;
  virtual void setListEditorData(QWidget* editor, const QVariant&) const = 0;

  void setMapping(QStringList names, QVariantList values = QVariantList());
  QStringList valueNames() const;
  QVariant valueData(const QString&) const;

  virtual void setModelData(QWidget* editor, const QString& text,
                            const QVariant& data, QAbstractItemModel* model,
                            const QModelIndex& index) const;

  virtual bool compareData(const QVariant&, const QVariant&) const;

private:
  QTE_DECLARE_PRIVATE(qtAbstractListDelegate)
  QTE_DISABLE_COPY(qtAbstractListDelegate)
};

#endif
