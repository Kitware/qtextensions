/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
