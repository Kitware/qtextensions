/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtPrioritizedToolBarProxy_h
#define __qtPrioritizedToolBarProxy_h

#include <QScopedPointer>

#include "../core/qtGlobal.h"

#include "qtUtilNamespace.h"

class QToolBar;
class QAction;
class QWidget;

class qtPrioritizedToolBarProxyPrivate;

/// Tool bar proxy supporting prioritized insertions.
///
/// This class wraps a QToolBar to provide prioritized insertion of actions
/// into the tool bar. This is mainly of use to applications implementing
/// plugin interfaces that wish to allow the plugins to add actions and/or
/// widgets to tool bars, but do not want to deal with prioritizing calls to
/// the plugins in order to maintain ordering consistency. The use of a
/// prioritized proxy allows plugins to specify a unique priority in order to
/// ensure consistent ordering of tool bar actions and widgets.
class QTE_EXPORT qtPrioritizedToolBarProxy
{
public:
  /// Create proxy.
  ///
  /// \param toolBar QToolBar which this qtPrioritizedToolBarProxy wraps.
  /// \param followingAction
  ///   Action before which action inserted by this proxy will be inserted. If
  ///   omitted or 0, actions and widgets will be added to the end of the tool
  ///   bar.
  /// \param separators
  ///   Flags specifying if and where separators should be added to the menu
  ///   when actions are inserted via the proxy.
  qtPrioritizedToolBarProxy(
    QToolBar* toolBar, QAction* followingAction = 0,
    qtUtil::SeparatorBehavior separators = qtUtil::NoSeparators);

  ~qtPrioritizedToolBarProxy();

  /// Get the QToolBar associated with this proxy.
  const QToolBar* toolBar() const;

  /// Insert action.
  ///
  /// This inserts the specified QAction into the tool bar with the specified
  /// priority. The action will be inserted after actions of lesser priority,
  /// and before actions of equal or greater priority.
  void insertAction(QAction*, int priority = 0);

  /// Insert separator.
  ///
  /// This inserts a separator into the tool bar with the specified priority.
  /// The separator will be inserted after actions of lesser priority, and
  /// before actions of equal or greater priority.
  ///
  /// \return The QAction which wraps the separator in the tool bar.
  QAction* insertSeparator(int priority = 0);

  /// Insert widget.
  ///
  /// This inserts the specified QWidget into the tool bar with the specified
  /// priority. The widget will be inserted after actions of lesser priority,
  /// and before actions of equal or greater priority.
  ///
  /// \return The QAction which wraps the widget in the tool bar.
  QAction* insertWidget(QWidget*, int priority = 0);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtPrioritizedToolBarProxy)

private:
  QTE_DECLARE_PRIVATE(qtPrioritizedToolBarProxy)
  QTE_DISABLE_COPY(qtPrioritizedToolBarProxy)
};

#endif
