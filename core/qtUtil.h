// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtUtil_h
#define __qtUtil_h

#include "qtGlobal.h"

class QDialogButtonBox;
class QIcon;
class QMainWindow;
class QString;
class QStringList;
class QTreeWidget;
class QWidget;

template <typename T> class QList;

namespace qtUtil
{
  /// Sort a list of strings in a locale-aware manner.
  ///
  /// This function sorts a ::QStringList using QString::localeAwareCompare as
  /// the comparison function.
  QTE_EXPORT QStringList localeSort(const QStringList&);

  /// Retrieve icon resource by name.
  ///
  /// This function retrieves an icon from an applications resource pool based
  /// on the icon's short name, assuming that the resource name follows the
  /// standard template ':icons/<i>size</i>x<i>size</i>/<i>name</i>. The
  /// returned icon has a single pixmap matching the requested size.
  ///
  /// \param name Short name of the icon to retrieve.
  /// \param size Size to retrieve.
  QTE_EXPORT QIcon standardIcon(const QString& name, int size);

  /// Retrieve icon resource by name.
  ///
  /// This function retrieves an icon from an applications resource pool based
  /// on the icon's short name. It will attempt to load multiple pixmaps for
  /// the icon matching the list of requested sizes.
  ///
  /// \param name Short name of the icon to retrieve.
  /// \param sizes List of sizes to retrieve.
  QTE_EXPORT QIcon standardIcon(const QString& name, QList<int> sizes);

  /// Retrieve action icon resource by name.
  ///
  /// This function retrieves an icon from an applications resource pool based
  /// on the icon's short name, automatically loading multiple pixmaps for
  /// standard action icon resolutions.
  ///
  /// \param name Short name of the icon to retrieve.
  ///
  /// \sa qtUtil::standardIcon
  QTE_EXPORT QIcon standardActionIcon(const QString& name);

  /// Retrieve application icon resource by name.
  ///
  /// This function retrieves an icon from an applications resource pool based
  /// on the icon's short name, automatically loading multiple pixmaps for
  /// standard application icon resolutions.
  ///
  /// \param name Short name of the icon to retrieve.
  ///
  /// \sa qtUtil::standardIcon
  QTE_EXPORT QIcon standardApplicationIcon(const QString& name);

  /// Apply "standard" action icons to dialog buttons.
  ///
  /// This function applies the "standard" icons (as returned by
  /// standardActionIcon) to the standard buttons of a QDialogButtonBox. This
  /// provides greater consistency for applications that use their own
  /// "standard" icons for okay, cancel, etc. rather than the system icons.
  QTE_EXPORT void setStandardIcons(QDialogButtonBox*);

  QTE_EXPORT void setApplicationIcon(const QString& name, QMainWindow* = 0);

  QTE_EXPORT void resizeColumnsToContents(QTreeWidget*,
                                          bool includeCollapsedItems = true);

  /// Make widget transparent.
  ///
  /// This function makes a widget "transparent" by setting the background
  /// brush used to paint the widget's background (as determined by
  /// QWidget::backgroundRole()) to a default-constructed (transparent) brush.
  /// For most widgets, this will result in the widget not painting a
  /// background, making it effectively transparent.
  QTE_EXPORT void makeTransparent(QWidget*);
}

#endif
