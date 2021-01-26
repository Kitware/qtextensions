// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
#include <QProcess>
#endif

#include <algorithm>

namespace qtUtil
{

//-----------------------------------------------------------------------------
bool localeLess(QString const& a, QString const& b)
{
    return a.localeAwareCompare(b) < 0;
}

//-----------------------------------------------------------------------------
QStringList localeSort(QStringList const& list)
{
    auto result = list;
    std::sort(result.begin(), result.end(), &localeLess);
    return result;
}

//-----------------------------------------------------------------------------
QString standardIconResource(QString const& name, int size)
{
    return QString(":icons/%1x%1/%2").arg(size).arg(name);
}

//-----------------------------------------------------------------------------
QIcon standardIcon(QString const& name, int size)
{
    return QIcon{standardIconResource(name, size)};
}

//-----------------------------------------------------------------------------
QIcon standardIcon(QString const& name, QList<int> sizes)
{
    QIcon icon;
    foreach (auto const size, sizes)
        icon.addFile(standardIconResource(name, size));
    return icon;
}

//-----------------------------------------------------------------------------
QIcon standardActionIcon(QString const& name)
{
    auto icon = QIcon{standardIconResource(name, 16)};
    icon.addFile(standardIconResource(name, 22));
    icon.addFile(standardIconResource(name, 24));
    icon.addFile(standardIconResource(name, 32));
    return icon;
}

//-----------------------------------------------------------------------------
QIcon standardApplicationIcon(QString const& name)
{
    auto icon = QIcon{standardIconResource(name, 16)};
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
            buttons->button(which)->setIcon(icon);
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
void setApplicationIcon(QString const& name, QMainWindow* mainWindow)
{
    auto const& icon = standardApplicationIcon(name);
    qApp->setWindowIcon(icon);
    if (mainWindow)
        mainWindow->setWindowIcon(icon);
}

//-----------------------------------------------------------------------------
void setIconTheme(QString const& themeName)
{
    auto const& originalTheme = QIcon::themeName();
    QIcon::setThemeName(themeName);

  // Set fallback theme
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    if (!originalTheme.isEmpty())
    {
        QIcon::setFallbackThemeName(originalTheme);
    }
    else
    {
        // QIcon::themeName appears to have a bug that causes it to always
        // return an empty string; this is a grotesque work-around to get the
        // theme name
        QProcess p{qApp};
        p.start(
            QStringLiteral("gsettings"),
            {
                QStringLiteral("get"),
                QStringLiteral("org.gnome.desktop.interface"),
                QStringLiteral("icon-theme")
            });
        if (p.waitForFinished())
        {
            auto const& output =
                QString::fromLocal8Bit(p.readAllStandardOutput()).trimmed();
            auto const re = QRegularExpression{QStringLiteral("'([-\\w]+)'")};
            auto const m = re.match(output);
            if (m.isValid())
                QIcon::setFallbackThemeName(m.captured(1));
        }
    }
#endif
}

//-----------------------------------------------------------------------------
void resizeColumnsToContents(QTreeWidget* tree, bool includeCollapsedItems)
{
    auto collapsedItems = QList<QTreeWidgetItem*>{};

    // If including collapsed items, first expand the entire tree
    if (includeCollapsedItems)
    {
        for (QTreeWidgetItemIterator iter(tree); *iter; ++iter)
        {
            auto* const item = *iter;
            if (item->childCount() && !item->isExpanded())
            {
                item->setExpanded(true);
                collapsedItems.append(item);
            }
        }
    }

    auto i = tree->columnCount();
    while (i--)
    {
        // Resize to data
        tree->resizeColumnToContents(i);

        // Also resize to header, if applicable
        if (!tree->isHeaderHidden())
        {
            // Get current size
            auto const cw = tree->columnWidth(i);

            // Get header text and icon
            auto const& text = tree->headerItem()->text(i);
            auto const& icon = tree->headerItem()->icon(i);

            // Get header font, using widget font if unset
            auto font = tree->font();
            auto hfd = tree->headerItem()->data(i, Qt::FontRole);
            if (hfd.isValid() && hfd.canConvert(QVariant::Font))
                font = hfd.value<QFont>();

            // Calculate size of text and icon
            auto fm = QFontMetrics{font};
            auto tw = fm.boundingRect(text).width();
            if (!icon.isNull())
                tw += icon.actualSize(tree->iconSize()).width();

            // Add space for margins and sort indicator
            tw += 2 * tree->style()->pixelMetric(QStyle::PM_HeaderMargin);
            tw += 2 * tree->style()->pixelMetric(QStyle::PM_HeaderGripMargin);
            tw += tree->style()->pixelMetric(QStyle::PM_HeaderMarkSize);

            // Use header size, if larger
            if (tw > cw)
                tree->setColumnWidth(i, tw);
        }
    }

    // Restore collapsed state of anything we expanded
    foreach (auto const item, collapsedItems)
        item->setExpanded(false);
}

//-----------------------------------------------------------------------------
void makeTransparent(QWidget* widget)
{
    auto const cr = widget->backgroundRole();
    auto palette = widget->palette();
    palette.setBrush(cr, QBrush{});
    widget->setPalette(palette);
}

} // namespace qtUtil
