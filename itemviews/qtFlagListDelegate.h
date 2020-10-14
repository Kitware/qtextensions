// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtFlagListDelegate_h
#define __qtFlagListDelegate_h

#include "qtListDelegate.h"

#include "../core/qtIndexRange.h"

#include <QListWidgetItem>

template <typename Flag, typename Flags>
class qtFlagListDelegate : public qtListDelegate
{
public:
  qtFlagListDelegate(QObject* parent = 0) : qtListDelegate(parent) {}
  virtual ~qtFlagListDelegate() {}

  virtual void setModelData(QWidget* editor, QAbstractItemModel*,
                            const QModelIndex&) const;

protected:
  using qtListDelegate::setModelData;

  virtual QListWidgetItem* createListItem(const QString& name,
                                          const QVariant& data) const;
  virtual void setListEditorData(QWidget* editor, const QVariant&) const;

  virtual bool compareData(const QVariant&, const QVariant&) const;

private:
  QTE_DISABLE_COPY(qtFlagListDelegate);
};

//-----------------------------------------------------------------------------
template <typename Flag, typename Flags>
QListWidgetItem* qtFlagListDelegate<Flag, Flags>::createListItem(
  const QString& name, const QVariant& data) const
{
  QListWidgetItem* item = qtListDelegate::createListItem(name, data);
  item->setCheckState(Qt::Unchecked);
  item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
  return item;
}

//-----------------------------------------------------------------------------
template <typename Flag, typename Flags>
void qtFlagListDelegate<Flag, Flags>::setListEditorData(
  QWidget* editor, const QVariant& data) const
{
  if (data.canConvert<Flags>())
    {
    QListWidget* list = qobject_cast<QListWidget*>(editor);
    Flags dataFlags = data.value<Flags>();

    // Set check state of appropriate items
    foreach (auto const i, qtIndexRange(list->count()))
      {
      QListWidgetItem* item = list->item(i);
      bool check = false;
      QVariant itemData = item->data(Qt::UserRole);
      if (itemData.canConvert<Flag>())
        {
        Flag itemFlag = itemData.value<Flag>();
        check = dataFlags.testFlag(itemFlag);
        }
      item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
      }
    }
}

//-----------------------------------------------------------------------------
template <typename Flag, typename Flags>
void qtFlagListDelegate<Flag, Flags>::setModelData(
  QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QListWidget* list = qobject_cast<QListWidget*>(editor);
  QStringList flagTexts;
  Flags newData = 0;

  // Generate text and data
  foreach (auto const i, qtIndexRange(list->count()))
    {
    QListWidgetItem* item = list->item(i);
    if (item->checkState())
      {
      QVariant itemData = item->data(Qt::UserRole);
      if (itemData.canConvert<Flag>())
        {
        newData |= itemData.value<Flag>();
        flagTexts.append(item->text());
        }
      }
    }

  // Update model
  QString text = (flagTexts.isEmpty() ? "(None)" : flagTexts.join(", "));
  qtListDelegate::setModelData(editor, text,
                               QVariant::fromValue<Flags>(newData),
                               model, index);
}

//-----------------------------------------------------------------------------
template <typename Flag, typename Flags>
bool qtFlagListDelegate<Flag, Flags>::compareData(
  const QVariant& a, const QVariant& b) const
{
  if (a.canConvert<Flag>() && b.canConvert<Flag>())
    {
    return a.value<Flag>() == b.value<Flag>();
    }
  else if (a.canConvert<Flag>() && b.canConvert<Flag>())
    {
    return a.value<Flag>() == b.value<Flag>();
    }
  return qtListDelegate::compareData(a, b);
}

#endif
