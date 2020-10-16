// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtPrioritizedMenuProxy_h
#define __qtPrioritizedMenuProxy_h

#include <QScopedPointer>

#include "../core/qtGlobal.h"

#include "qtUtilNamespace.h"

class QMenu;
class QAction;

class qtPrioritizedMenuProxyPrivate;

/// Menu proxy supporting prioritized insertions.
///
/// This class wraps a QMenu to provide prioritized insertion of actions into
/// the menu. This is mainly of use to applications implementing plugin
/// interfaces that wish to allow the plugins to add actions to menus, but do
/// not want to deal with prioritizing calls to the plugins in order to
/// maintain ordering consistency. The use of a prioritized proxy allows
/// plugins to specify a unique priority in order to ensure consistent ordering
/// of menu actions.
class QTE_EXPORT qtPrioritizedMenuProxy
{
public:
  /// Create proxy.
  ///
  /// \param menu QMenu which this qtPrioritizedMenuProxy wraps.
  /// \param followingAction
  ///   Action before which action inserted by this proxy will be inserted. If
  ///   omitted or 0, actions will be added to the end of the menu.
  /// \param separators
  ///   Flags specifying if and where separators should be added to the menu
  ///   when actions are inserted via the proxy.
  qtPrioritizedMenuProxy(
    QMenu* menu, QAction* followingAction = 0,
    qtUtil::SeparatorBehavior separators = qtUtil::NoSeparators);

  ~qtPrioritizedMenuProxy();

  /// Get the QMenu associated with this proxy.
  const QMenu* menu() const;

  /// Insert action.
  ///
  /// This inserts the specified QAction into the menu with the specified
  /// priority. The action will be inserted after actions of lesser priority,
  /// and before actions of equal or greater priority.
  void insertAction(QAction*, int priority = 0);

  /// Insert separator.
  ///
  /// This inserts a separator into the menu with the specified priority. The
  /// separator will be inserted after actions of lesser priority, and before
  /// actions of equal or greater priority.
  ///
  /// \return The QAction which wraps the separator in the menu.
  QAction* insertSeparator(int priority = 0);

  /// Insert menu.
  ///
  /// This inserts the specified QMenu into the menu with the specified
  /// priority. The menu will be inserted after actions of lesser priority,
  /// and before actions of equal or greater priority.
  ///
  /// \return The QAction which wraps the child menu in the parent menu.
  QAction* insertMenu(QMenu*, int priority = 0);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtPrioritizedMenuProxy)

private:
  QTE_DECLARE_PRIVATE(qtPrioritizedMenuProxy)
  QTE_DISABLE_COPY(qtPrioritizedMenuProxy)
};

#endif
