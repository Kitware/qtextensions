/*ckwg +5
 * Copyright 2019 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtUiState_h
#define __qtUiState_h

#include <QScopedPointer>

class QAbstractButton;
class QAction;
class QDoubleSpinBox;
class QGroupBox;
class QHeaderView;
class QLineEdit;
class QMainWindow;
class QSettings;
class QSpinBox;
class QSplitter;
class QString;
class QStringList;
class QWidget;

#include "../core/qtGlobal.h"

class QVariant;

class qtUiStatePrivate;

//-----------------------------------------------------------------------------
/// Save and restore UI state.
///
/// qtUiState functions as a UI state container, allowing users to easily save
/// and restore UI statue with minimal code duplication.
///
/// Traditional state management uses direct calls to QSettings::value and
/// QSettings::setValue. In addition to duplicating key strings (with
/// corresponding potential for errors), the various steps needed to convert
/// from QVariant, interact with the appropriate widget property, and define
/// default values can result in lengthy code.
///
/// qtUiState allows the user to instead define a map of widget-property pairs
/// to setting keys. Once this is done, saving and restoring of settings can be
/// done with a single method. Additionally, the built-in map handlers use the
/// widget's current property value as the default, allowing default-value
/// changes to be made in .ui files (where used) rather than code.
///
/// \section key_naming Key Naming
///
/// Like QSettings, qtUiState supports storing of data in a tree structure.
/// Each qtUiState instance has a <em>current group</em> that is prepended to
/// any non-absolute key names when creating a mapping. Key names may specify
/// zero or more tree levels by separating each level with '/'. Keys starting
/// with '/' are \em absolute, and ignore the current group.
///
/// When matching key names:
/// \li A key specification starting with '/' matches keys or groups starting
///     with the specified text.
/// \li A key specification ending with '/' matches all keys within a group
///     that matches the specified text.
/// \li A key specification neither starting nor ending with '/' matches all
///     keys and groups that contain the specified text.
///
/// Matching is always performed on complete group names. That is, the
/// specification "bar" matches keys "sand/bar" and "bar/none", but not
/// "fubar".
class QTE_EXPORT qtUiState
{
public:
  /// Abstract class to save and restore UI state of an object.
  ///
  /// qtUiState::AbstractItem defines an interface through which qtUiState can
  /// save or restore the state of any object that provides an implementation
  /// of such interface. Marshaling the data to/from QSettings is handled by
  /// qtUiState; the interface is only responsible for creating the data from
  /// the object's state and using the data to restore the object's state. As
  /// qtUiState is not responsible for managing the object itself, most
  /// implementations will have an internal pointer to the object.
  class AbstractItem
    {
    public:
      virtual ~AbstractItem() {}

      /// Serialize the object's state.
      /// \return A QVariant representing the object's state.
      virtual QVariant value() const = 0;

      /// Restore the object's state.
      /// \param data A QVariant containing the serialized representation of
      ///             the object's state.
      virtual void setValue(const QVariant& data) = 0;
    };

  // See qtUiStateItem.h
  template <typename ValueType, typename Object> class Item;

  /// Construct a qtUiState, using the specified QSettings as the backing
  /// store.
  ///
  /// If \p store is non-null, the qtUiState takes ownership of the QSettings
  /// object and uses it to save and restore the UI state. Otherwise, qtUiState
  /// uses a default-constructed QSettings.
  explicit qtUiState(QSettings* store = 0);

  virtual ~qtUiState();

  /// Save settings for all items.
  ///
  /// This method saves the settings for all items registered with the
  /// qtUiState instance.
  void save() const;
  /// Save settings for named item(s).
  ///
  /// This method saves the settings for one or more items matching the
  /// specified \p key, as described in \ref key_naming.
  void save(const QString& key) const;
  /// \copydoc save(const QString&) const
  void save(const QStringList& keys) const;

  /// Restore settings for all items.
  ///
  /// This method restores the settings for all items registered with the
  /// qtUiState instance.
  void restore() const;
  /// Restore settings for named item(s).
  ///
  /// This method restores the settings for one or more items matching the
  /// specified \p keys, as described in \ref key_naming.
  void restore(const QString& key) const;
  /// \copydoc restore(const QString&) const
  void restore(const QStringList& keys) const;

  /// Get current settings group.
  ///
  /// This method returns the current settings group. The current group is
  /// prefixed to all non-absolute keys when defining a new mapping.
  QString currentGroup();
  /// Set current settings group.
  ///
  /// This method sets the current settings group. The current group is
  /// prefixed to all non-absolute keys when defining a new mapping.
  void setCurrentGroup(const QString&);

  /// Define mapping of a QAction's checked state.
  void mapChecked(const QString& key, QAction*);
  /// Define mapping of a QAbstractButton's checked state.
  void mapChecked(const QString& key, QAbstractButton*);
  /// Define mapping of a QGroupBox's checked state.
  void mapChecked(const QString& key, QGroupBox*);

  /// Define mapping of a QLineEdit's text property.
  void mapText(const QString& key, QLineEdit*);

  /// Define mapping of a QSpinBox's value property.
  void mapValue(const QString& key, QSpinBox*);
  /// Define mapping of a QDoubleSpinBox's value property.
  void mapValue(const QString& key, QDoubleSpinBox*);

  /// Define mapping of a QMainWindow's state.
  ///
  /// This method defines a mapping to save and restore the state (position and
  /// layout of tool bars and dock widgets) of a QMainWindow.
  /// \sa QMainWindow::saveState, QMainWindow::restoreState
  void mapState(const QString& key, QMainWindow*, int version = 0);
  /// Define mapping of a QSplitter's state.
  ///
  /// This method defines a mapping to save and restore the state (size and
  /// layout of children) of a QSplitter.
  /// \sa QSplitter::saveState, QSplitter::restoreState
  void mapState(const QString& key, QSplitter*);
  /// Define mapping of a QHeaderView's state.
  ///
  /// This method defines a mapping to save and restore the state (section
  /// sizes and sorting) of a QHeaderView.
  /// \sa QHeaderView::saveState, QHeaderView::restoreState
  void mapState(const QString& key, QHeaderView*);

  /// Define mapping of a widget's geometry.
  ///
  /// This method defines a mapping to save and restore the geometry of a
  /// QWidget. While this can be used for any widget, it would be most commonly
  /// used for an application window or dialog.
  void mapGeometry(const QString& key, QWidget*);

  /// Map the state of a generic object.
  ///
  /// The qtUiState instance assumes ownership of \p mapper.
  ///
  /// \param mapper Pointer to an implementation of qtUiState::AbstractItem
  ///               that encapsulates the state management of an object.
  void map(const QString& key, AbstractItem* mapper);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtUiState)

private:
  QTE_DECLARE_PRIVATE(qtUiState)
  QTE_DISABLE_COPY(qtUiState)
};

#endif
