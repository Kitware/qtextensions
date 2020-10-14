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
  qtComboBoxDelegate(QObject* parent = 0);
  virtual ~qtComboBoxDelegate();

  virtual void setModelData(QWidget* editor, QAbstractItemModel*,
                            const QModelIndex&) const;

  virtual void updateEditorGeometry(QWidget* editor,
                                    const QStyleOptionViewItem&,
                                    const QModelIndex&) const;

protected:
  using qtAbstractListDelegate::setModelData;

  virtual QWidget* createListEditor(QWidget* parent) const;
  virtual void setListEditorData(QWidget* editor, const QVariant&) const;

private:
  QTE_DISABLE_COPY(qtComboBoxDelegate);
};

#endif
