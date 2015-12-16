/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtAbstractListDelegate.h"

#include "../core/qtIndexRange.h"
#include "../core/qtScopedValueChange.h"

QTE_IMPLEMENT_D_FUNC(qtAbstractListDelegate)

//-----------------------------------------------------------------------------
class qtAbstractListDelegatePrivate
{
public:
  QStringList names;
  QHash<QString, QVariant> dataMap;
};

//-----------------------------------------------------------------------------
qtAbstractListDelegate::qtAbstractListDelegate(QObject* parent)
  : QStyledItemDelegate(parent), d_ptr(new qtAbstractListDelegatePrivate)
{
}

//-----------------------------------------------------------------------------
qtAbstractListDelegate::~qtAbstractListDelegate()
{
}

//-----------------------------------------------------------------------------
void qtAbstractListDelegate::setMapping(
  QStringList names, QVariantList values)
{
  QTE_D(qtAbstractListDelegate);
  d->names = names;
  d->dataMap.clear();
  foreach (auto const i, qtIndexRange(names.count()))
    {
    QVariant v = (i < values.count() ? values[i] : QVariant(i));
    d->dataMap.insert(names[i], v);
    }
}

//-----------------------------------------------------------------------------
bool qtAbstractListDelegate::compareData(
  const QVariant& a, const QVariant& b) const
{
  return a == b;
}

//-----------------------------------------------------------------------------
QStringList qtAbstractListDelegate::valueNames() const
{
  QTE_D_CONST(qtAbstractListDelegate);
  return d->names;
}

//-----------------------------------------------------------------------------
QVariant qtAbstractListDelegate::valueData(const QString& name) const
{
  QTE_D_CONST(qtAbstractListDelegate);
  return d->dataMap.value(name);
}

//-----------------------------------------------------------------------------
QWidget* qtAbstractListDelegate::createEditor(
  QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
{
  QWidget* editor = this->createListEditor(parent);
  editor->setProperty("ignoreUpdate", false);
  return editor;
}

//-----------------------------------------------------------------------------
void qtAbstractListDelegate::setEditorData(
  QWidget* editor, const QModelIndex& index) const
{
  if (editor->property("ignoreUpdate").toBool())
    {
    return;
    }

  QVariant data = index.model()->data(index, Qt::UserRole);
  this->setListEditorData(editor, data);
}

//-----------------------------------------------------------------------------
void qtAbstractListDelegate::setModelData(
  QWidget* editor, const QString& newText, const QVariant& newData,
  QAbstractItemModel* model, const QModelIndex& index) const
{
  // Update the model
  model->setData(index, newText, Qt::DisplayRole);
  model->setData(index, newData, Qt::UserRole);

  // Check if the model rejected or altered the change
  if (!this->compareData(model->data(index, Qt::UserRole), newData))
    {
    // Reset the editor, disabling ignoreUpdate since we're likely being called
    // by way of valueChanged()
    qtScopedPropertyChange p(editor, "ignoreUpdate", false);
    this->setEditorData(editor, index);
    }
}

//-----------------------------------------------------------------------------
void qtAbstractListDelegate::editorValueChanged()
{
  // Get the editor
  QWidget* editor = qobject_cast<QWidget*>(this->sender());
  if (!editor)
    {
    return;
    }

  // Commit the changed data on value selection
  qtScopedPropertyChange p(editor, "ignoreUpdate", true);
  emit this->commitData(editor);
}
