// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtComboBoxDelegate.h"

#include "../core/qtIndexRange.h"

#include <QComboBox>

//-----------------------------------------------------------------------------
qtComboBoxDelegate::qtComboBoxDelegate(QObject* parent)
  : qtAbstractListDelegate(parent)
{
}

//-----------------------------------------------------------------------------
qtComboBoxDelegate::~qtComboBoxDelegate()
{
}

//-----------------------------------------------------------------------------
QWidget* qtComboBoxDelegate::createListEditor(QWidget* parent) const
{
  // Create combo box
  QComboBox* box = new QComboBox(parent);
  box->setFocusPolicy(Qt::StrongFocus);
  connect(box, SIGNAL(currentIndexChanged(int)),
          this, SLOT(editorValueChanged()));

  // Fill combo box
  foreach (auto const& name, this->valueNames())
    box->addItem(name, this->valueData(name));

  // Flag box to show pop-up once we know the geometry
  box->setProperty("firstShow", true);

  // Done
  return box;
}

//-----------------------------------------------------------------------------
void qtComboBoxDelegate::setListEditorData(
  QWidget* editor, const QVariant& newData) const
{
  QComboBox* box = qobject_cast<QComboBox*>(editor);

  int i = -1;
  foreach (int const j, qtIndexRange(box->count()))
    {
    if (this->compareData(newData, box->itemData(j)))
      {
      i = j;
      break;
      }
    }
  box->setCurrentIndex(i);
}

//-----------------------------------------------------------------------------
void qtComboBoxDelegate::setModelData(
  QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QComboBox* box = qobject_cast<QComboBox*>(editor);
  QVariant newData = box->itemData(box->currentIndex());
  this->setModelData(editor, box->currentText(), newData, model, index);
}

//-----------------------------------------------------------------------------
void qtComboBoxDelegate::updateEditorGeometry(
  QWidget* editor,
  const QStyleOptionViewItem& option,
  const QModelIndex&) const
{
  QComboBox* box = qobject_cast<QComboBox*>(editor);
  box->setGeometry(option.rect);

  // Show pop-up at creation after the geometry is initialized
  if (box->property("firstShow").toBool())
    {
    box->showPopup();
    box->setProperty("firstShow", false);
    }
}
