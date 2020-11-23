// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtComboBoxDelegate_h
#define __qtComboBoxDelegate_h

#include "qtAbstractListDelegate.h"

class QTE_EXPORT qtComboBoxDelegate : public qtAbstractListDelegate
{
  Q_OBJECT

public:
    qtComboBoxDelegate(QObject* parent = nullptr);
    virtual ~qtComboBoxDelegate();

    virtual void setModelData(
        QWidget* editor, QAbstractItemModel* model,
        QModelIndex const& index) const override;

    virtual void updateEditorGeometry(
        QWidget* editor, QStyleOptionViewItem const& opt,
        QModelIndex const& index) const override;

protected:
  using qtAbstractListDelegate::setModelData;

  virtual QWidget* createListEditor(QWidget* parent) const override;
  virtual void setListEditorData(
      QWidget* editor, QVariant const& newData) const override;

private:
  QTE_DISABLE_COPY(qtComboBoxDelegate);
};

#endif
