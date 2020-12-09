// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtListDelegate_h
#define __qtListDelegate_h

#include "qtAbstractListDelegate.h"

class QListWidgetItem;

class QTE_EXPORT qtListDelegate : public qtAbstractListDelegate
{
  Q_OBJECT

public:
  qtListDelegate(QObject* parent = nullptr);
  virtual ~qtListDelegate();

  virtual void setModelData(QWidget* editor, QAbstractItemModel*,
                            const QModelIndex&) const;
  virtual void updateEditorGeometry(QWidget* editor,
                                    const QStyleOptionViewItem&,
                                    const QModelIndex&) const;

protected:
  using qtAbstractListDelegate::setModelData;

  virtual QWidget* createListEditor(QWidget* parent) const;
  virtual QListWidgetItem* createListItem(const QString& name,
                                          const QVariant& data) const;
  virtual void setListEditorData(QWidget* editor, const QVariant&) const;

private:
  QTE_DISABLE_COPY(qtListDelegate);
};

#endif
