/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtActionManagerDialog.h"
#include "ui_qtActionManagerDialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHash>
#include <QLabel>
#include <QLineEdit>

#include "../core/qtUtil.h"

#include "../util/qtActionManager.h"

QTE_IMPLEMENT_D_FUNC(qtActionManagerDialog)

//-----------------------------------------------------------------------------
class qtActionManagerDialogPrivate
{
public:
  Ui::qtActionManagerDialog UI;
};

//-----------------------------------------------------------------------------
qtActionManagerDialog::qtActionManagerDialog(
  QWidget* parent, Qt::WindowFlags flags)
  : QDialog(parent, flags), d_ptr(new qtActionManagerDialogPrivate)
{
  QTE_D(qtActionManagerDialog);
  d->UI.setupUi(this);
  this->setWindowTitle("Configure Shortcuts - "
                       + QApplication::applicationName());

  d->UI.clearFilter->setIcon(qtUtil::standardIcon("clear-lineedit", 16));
  qtUtil::setStandardIcons(d->UI.buttonBox);

  connect(d->UI.filter, SIGNAL(textEdited(QString)),
          this, SLOT(filterChanged(QString)));
  connect(d->UI.clearFilter, SIGNAL(clicked()),
          this, SLOT(filterChanged()));

  if (qtAm)
    {
    qtAm->addDynamicActions(this);
    }
}

//-----------------------------------------------------------------------------
qtActionManagerDialog::~qtActionManagerDialog()
{
  delete this->d_ptr;
}

//-----------------------------------------------------------------------------
void qtActionManagerDialog::registerAction(
  QString key, QString displayGroup, QString text, QString icon,
  QKeySequence defaultShortcut)
{
  QList<QKeySequence> defaultShortcuts;
  defaultShortcuts.append(defaultShortcut);
  this->registerAction(key, displayGroup, text, icon, defaultShortcuts);
}

//-----------------------------------------------------------------------------
void qtActionManagerDialog::registerAction(
  QString key, QString displayGroup, QString text, QString icon,
  QList<QKeySequence> defaultShortcut)
{
  // \TODO not implemented yet!
  Q_UNUSED(key);
  Q_UNUSED(displayGroup);
  Q_UNUSED(text);
  Q_UNUSED(icon);
  Q_UNUSED(defaultShortcut);
}

//-----------------------------------------------------------------------------
void qtActionManagerDialog::filterChanged(QString text)
{
  QTE_D(qtActionManagerDialog);
  d->UI.clearFilter->setEnabled(!text.isEmpty());
}
