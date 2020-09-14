/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtUtil.h"

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFontMetrics>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>

#include <algorithm>

namespace qtUtil
{

//-----------------------------------------------------------------------------
bool localeLess(const QString& a, const QString& b)
{
  return a.localeAwareCompare(b) < 0;
}

//-----------------------------------------------------------------------------
QStringList localeSort(const QStringList& list)
{
  QStringList result = list;
  std::sort(result.begin(), result.end(), &localeLess);
  return result;
}

//-----------------------------------------------------------------------------
QString standardIconResource(const QString& name, int size)
{
  return QString(":icons/%1x%1/%2").arg(size).arg(name);
}

//-----------------------------------------------------------------------------
QIcon standardIcon(const QString& name, int size)
{
  return QIcon{standardIconResource(name, size)};
}

//-----------------------------------------------------------------------------
QIcon standardIcon(const QString& name, QList<int> sizes)
{
  QIcon icon;
  foreach (auto const size, sizes)
    icon.addFile(standardIconResource(name, size));
  return icon;
}

//-----------------------------------------------------------------------------
QIcon standardActionIcon(const QString& name)
{
  QIcon icon(standardIconResource(name, 16));
  icon.addFile(standardIconResource(name, 22));
  icon.addFile(standardIconResource(name, 24));
  icon.addFile(standardIconResource(name, 32));
  return icon;
}

//-----------------------------------------------------------------------------
QIcon standardApplicationIcon(const QString& name)
{
  QIcon icon(standardIconResource(name, 16));
  icon.addFile(standardIconResource(name, 32));
  icon.addFile(standardIconResource(name, 48));
  icon.addFile(standardIconResource(name, 64));
  icon.addFile(standardIconResource(name, 128));
  return icon;
}

//-----------------------------------------------------------------------------
void setStandardIcon(
  QDialogButtonBox* buttons,
  QDialogButtonBox::StandardButton which,
  QString name)
{
  if (buttons->button(which))
    {
    QIcon icon = standardActionIcon(name);
    if (icon.availableSizes().count())
      {
      buttons->button(which)->setIcon(icon);
      }
    }
}

//-----------------------------------------------------------------------------
void setStandardIcons(QDialogButtonBox* buttons)
{
  setStandardIcon(buttons, QDialogButtonBox::Ok, "okay");
  setStandardIcon(buttons, QDialogButtonBox::Cancel, "cancel");
  setStandardIcon(buttons, QDialogButtonBox::Apply, "apply");
  setStandardIcon(buttons, QDialogButtonBox::Reset, "reset");
  setStandardIcon(buttons, QDialogButtonBox::RestoreDefaults, "reset");
  setStandardIcon(buttons, QDialogButtonBox::Close, "close");
}

//-----------------------------------------------------------------------------
void setApplicationIcon(const QString& name, QMainWindow* mainWindow)
{
  const QIcon icon = standardApplicationIcon(name);
  qApp->setWindowIcon(icon);
  mainWindow && (mainWindow->setWindowIcon(icon), false);
}

//-----------------------------------------------------------------------------
void resizeColumnsToContents(QTreeWidget* tree, bool includeCollapsedItems)
{
  QList<QTreeWidgetItem*> collapsedItems;

  // If including collapsed items, first expand the entire tree
  if (includeCollapsedItems)
    {
    for (QTreeWidgetItemIterator iter(tree); *iter; ++iter)
      {
      QTreeWidgetItem* item = *iter;
      if (item->childCount() && !item->isExpanded())
        {
        item->setExpanded(true);
        collapsedItems.append(item);
        }
      }
    }

  int i = tree->columnCount();
  while (i--)
    {
    // Resize to data
    tree->resizeColumnToContents(i);

    // Also resize to header, if applicable
    if (!tree->isHeaderHidden())
      {
      // Get current size
      int cw = tree->columnWidth(i);

      // Get header text and icon
      QString text = tree->headerItem()->text(i);
      QIcon icon = tree->headerItem()->icon(i);

      // Get header font, using widget font if unset
      QFont font = tree->font();
      QVariant hfd = tree->headerItem()->data(i, Qt::FontRole);
      if (hfd.isValid() && hfd.canConvert(QVariant::Font))
        {
        font = hfd.value<QFont>();
        }

      // Calculate size of text and icon
      QFontMetrics fm(font);
      int tw = fm.boundingRect(text).width();
      if (!icon.isNull())
        {
        tw += icon.actualSize(tree->iconSize()).width();
        }

      // Add space for margins and sort indicator
      tw += 2 * tree->style()->pixelMetric(QStyle::PM_HeaderMargin);
      tw += 2 * tree->style()->pixelMetric(QStyle::PM_HeaderGripMargin);
      tw += tree->style()->pixelMetric(QStyle::PM_HeaderMarkSize);

      // Use header size, if larger
      if (tw > cw)
        {
        tree->setColumnWidth(i, tw);
        }
      }
    }

  // Restore collapsed state of anything we expanded
  foreach (auto const item, collapsedItems)
    item->setExpanded(false);
}

//-----------------------------------------------------------------------------
void makeTransparent(QWidget* widget)
{
  QPalette::ColorRole cr = widget->backgroundRole();
  QPalette palette = widget->palette();
  palette.setBrush(cr, QBrush());
  widget->setPalette(palette);
}

} // namespace qtUtil
