/*ckwg +5
 * Copyright 2019 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtUiState.h"
#include "qtUiStateItem.h"

#include "qtHeaderView.h"

#include <QAbstractButton>
#include <QAction>
#include <QDebug>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHash>
#include <QHeaderView>
#include <QLineEdit>
#include <QMainWindow>
#include <QRegExp>
#include <QSettings>
#include <QSpinBox>
#include <QSplitter>
#include <QStringList>

QTE_IMPLEMENT_D_FUNC(qtUiState)

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtUiStatePrivate

//-----------------------------------------------------------------------------
class qtUiStatePrivate
{
public:
  template <typename O> class StateItem;

  qtUiStatePrivate(QSettings* s) : store(s ? s : new QSettings) {}
  ~qtUiStatePrivate();

  void map(QString key, qtUiState::AbstractItem* item);
  QString resolveKey(QString input) const;
  QStringList matchingKeys(const QString& pattern) const;

  void save(const QStringList& keys) const;
  void restore(const QStringList& keys) const;

  QString currentGroup;

  const QScopedPointer<QSettings> store;
  QHash<QString, qtUiState::AbstractItem*> items;
};

//-----------------------------------------------------------------------------
qtUiStatePrivate::~qtUiStatePrivate()
{
  qDeleteAll(this->items);
}

//-----------------------------------------------------------------------------
void qtUiStatePrivate::map(QString key, qtUiState::AbstractItem* item)
{
  key = this->resolveKey(key);

  if (this->items.contains(key))
    {
    qWarning() << "qtUiState: replacing existing mapping for" << key;
    delete this->items.take(key);
    }

  this->items.insert(key, item);
}

//-----------------------------------------------------------------------------
QString qtUiStatePrivate::resolveKey(QString input) const
{
  if (input.startsWith('/'))
    {
    while (input.startsWith('/'))
      {
      input = input.mid(1);
      }
    return input;
    }

  if (currentGroup.isEmpty())
    {
    return input;
    }

  return QString("%1/%2").arg(this->currentGroup, input);
}

//-----------------------------------------------------------------------------
QStringList qtUiStatePrivate::matchingKeys(const QString& pattern) const
{
  QRegExp re(
    (pattern.startsWith('/') ? "^" + pattern.mid(1) : "(^|/)" + pattern) +
    (pattern.endsWith('/') ? "" : "(/|$)"));
  QStringList matches;

  foreach (auto const& key, this->items.keys())
    {
    if (re.indexIn(key) >= 0)
      {
      matches.append(key);
      }
    }

  return matches;
}

//-----------------------------------------------------------------------------
void qtUiStatePrivate::save(const QStringList& keys) const
{
  foreach (auto const& key, keys)
    {
    qtUiState::AbstractItem* item = this->items.value(key, 0);
    if (item)
      {
      this->store->setValue(key, item->value());
      }
    }

  this->store->sync();
}

//-----------------------------------------------------------------------------
void qtUiStatePrivate::restore(const QStringList& keys) const
{
  this->store->sync();

  foreach (auto const& key, keys)
    {
    qtUiState::AbstractItem* item = this->items.value(key, 0);
    if (item)
      {
      item->setValue(this->store->value(key, item->value()));
      }
    }
}

//END qtUiStatePrivate

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtUiStatePrivate::StateItem

//-----------------------------------------------------------------------------
template <typename O>
class qtUiStatePrivate::StateItem : public qtUiState::AbstractItem
{
public:
  typedef QByteArray(O::*ReadMethod)() const;
  typedef QByteArray(O::*ReadVersionedMethod)(int) const;
  typedef bool (O::*WriteMethod)(const QByteArray&);
  typedef bool (O::*WriteVersionedMethod)(const QByteArray&, int);

  StateItem(O*, ReadMethod, WriteMethod);
  StateItem(O*, ReadVersionedMethod, WriteVersionedMethod, int);

  virtual QVariant value() const;
  virtual void setValue(const QVariant&);

protected:
  qtPointer<O> object;
  int version;
  const ReadMethod read;
  const ReadVersionedMethod readVersioned;
  const WriteMethod write;
  const WriteVersionedMethod writeVersioned;
};

//-----------------------------------------------------------------------------
template <typename O>
qtUiStatePrivate::StateItem<O>::StateItem(
  O* object, ReadMethod read, WriteMethod write)
  : object(object), version(0),
    read(read), readVersioned(0),
    write(write), writeVersioned(0)
{
}

//-----------------------------------------------------------------------------
template <typename O>
qtUiStatePrivate::StateItem<O>::StateItem(
  O* object, ReadVersionedMethod read, WriteVersionedMethod write, int version)
  : object(object), version(version),
    read(0), readVersioned(read),
    write(0), writeVersioned(write)
{
}

//-----------------------------------------------------------------------------
template <typename O>
QVariant qtUiStatePrivate::StateItem<O>::value() const
{
  if (!this->object)
    {
    return {};
    }

  if (this->read)
    {
    return (*(object.data()).*this->read)();
    }
  else
    {
    return (*(object.data()).*this->readVersioned)(this->version);
    }
}

//-----------------------------------------------------------------------------
template <typename O>
void qtUiStatePrivate::StateItem<O>::setValue(const QVariant& data)
{
  if (!this->object)
    {
    return;
    }

  if (this->write)
    {
    (*(object.data()).*this->write)(data.toByteArray());
    }
  else
    (*(object.data()).*this->writeVersioned)(data.toByteArray(),
                                             this->version);
}

//END qtUiStatePrivate::StateItem

///////////////////////////////////////////////////////////////////////////////

//BEGIN qtUiState

//-----------------------------------------------------------------------------
qtUiState::qtUiState(QSettings* s) : d_ptr(new qtUiStatePrivate(s))
{
}

//-----------------------------------------------------------------------------
qtUiState::~qtUiState()
{
}

//-----------------------------------------------------------------------------
void qtUiState::save() const
{
  QTE_D_CONST(qtUiState);
  d->save(d->items.keys());
}

//-----------------------------------------------------------------------------
void qtUiState::save(const QString& key) const
{
  QTE_D_CONST(qtUiState);
  d->save(d->matchingKeys(key));
}

//-----------------------------------------------------------------------------
void qtUiState::save(const QStringList& keys) const
{
  QTE_D_CONST(qtUiState);

  QSet<QString> matchingKeys;
  foreach (auto const& pattern, keys)
    {
    matchingKeys += QSet<QString>::fromList(d->matchingKeys(pattern));
    }
  d->save(matchingKeys.toList());
}

//-----------------------------------------------------------------------------
void qtUiState::restore() const
{
  QTE_D_CONST(qtUiState);
  d->restore(d->items.keys());
}

//-----------------------------------------------------------------------------
void qtUiState::restore(const QString& key) const
{
  QTE_D_CONST(qtUiState);
  d->restore(d->matchingKeys(key));
}

//-----------------------------------------------------------------------------
void qtUiState::restore(const QStringList& keys) const
{
  QTE_D_CONST(qtUiState);

  QSet<QString> matchingKeys;
  foreach (auto const& pattern, keys)
    {
    matchingKeys += QSet<QString>::fromList(d->matchingKeys(pattern));
    }
  d->restore(matchingKeys.toList());
}

//-----------------------------------------------------------------------------
QString qtUiState::currentGroup()
{
  QTE_D_CONST(qtUiState);
  return d->currentGroup;
}

//-----------------------------------------------------------------------------
void qtUiState::setCurrentGroup(const QString& group)
{
  QTE_D(qtUiState);
  d->currentGroup = group;
  while (d->currentGroup.startsWith('/'))
    {
    d->currentGroup = d->currentGroup.mid(1);
    }
  while (d->currentGroup.endsWith('/'))
    {
    d->currentGroup = d->currentGroup.left(d->currentGroup.size() - 1);
    }
}

//-----------------------------------------------------------------------------
void qtUiState::mapChecked(const QString& key, QAction* action)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiState::Item<bool, QAction>(
    action, &QAction::isChecked, &QAction::setChecked);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapChecked(const QString& key, QAbstractButton* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiState::Item<bool, QAbstractButton>(
    widget, &QAbstractButton::isChecked, &QAbstractButton::setChecked);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapChecked(const QString& key, QGroupBox* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiState::Item<bool, QGroupBox>(
    widget, &QGroupBox::isChecked, &QGroupBox::setChecked);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapText(const QString& key, QLineEdit* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiState::Item<QString, QLineEdit>(
    widget, &QLineEdit::text, &QLineEdit::setText);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapValue(const QString& key, QSpinBox* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiState::Item<int, QSpinBox>(
    widget, &QSpinBox::value, &QSpinBox::setValue);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapValue(const QString& key, QDoubleSpinBox* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiState::Item<double, QDoubleSpinBox>(
    widget, &QDoubleSpinBox::value, &QDoubleSpinBox::setValue);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapState(const QString& key, QMainWindow* window, int version)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiStatePrivate::StateItem<QMainWindow>(
    window, &QMainWindow::saveState, &QMainWindow::restoreState, version);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapState(const QString& key, QSplitter* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiStatePrivate::StateItem<QSplitter>(
    widget, &QSplitter::saveState, &QSplitter::restoreState);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapState(const QString& key, QHeaderView* view)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiStatePrivate::StateItem<QHeaderView>(
    view, &QHeaderView::saveState, &QHeaderView::restoreState);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapState(const QString& key, qtHeaderView* view)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiStatePrivate::StateItem<qtHeaderView>(
      view, &qtHeaderView::saveState, &qtHeaderView::restoreState);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::mapGeometry(const QString& key, QWidget* widget)
{
  QTE_D(qtUiState);
  qtUiState::AbstractItem* item =
    new qtUiStatePrivate::StateItem<QWidget>(
    widget, &QMainWindow::saveGeometry, &QMainWindow::restoreGeometry);
  d->map(key, item);
}

//-----------------------------------------------------------------------------
void qtUiState::map(const QString& key, qtUiState::AbstractItem* mapper)
{
  QTE_D(qtUiState);
  d->map(key, mapper);
}

//END qtUiState
