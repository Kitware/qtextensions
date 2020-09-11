/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtConfirmationDialog.h"
#include "ui_qtConfirmationDialog.h"

#include <QLayout>
#include <QPushButton>
#include <QSettings>
#include <QStyle>

#include "../core/qtUtil.h"

QTE_IMPLEMENT_D_FUNC(qtConfirmationDialog)

//-----------------------------------------------------------------------------
class qtConfirmationDialogPrivate
{
public:
  QString AskKey;
  qtConfirmationDialog::AskPreferenceScope Scope;
  Ui::qtConfirmationDialog UI;
};

//-----------------------------------------------------------------------------
qtConfirmationDialog::qtConfirmationDialog(
  const QString& askKey, QWidget* parent, Qt::WindowFlags flags)
  : QDialog(parent, flags), d_ptr(new qtConfirmationDialogPrivate)
{
  QTE_D(qtConfirmationDialog);

  d->AskKey = askKey;

  // Create UI
  d->UI.setupUi(this);
  this->setConfirmText("Continue");
  qtUtil::setStandardIcons(d->UI.buttonBox);

  // Set dialog icon
  QStyle* style = this->style();
  int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, 0, this);
  QIcon icon = style->standardIcon(QStyle::SP_MessageBoxWarning, 0, this);
  QPixmap pixmap = icon.pixmap(iconSize, iconSize);
  d->UI.image->setPixmap(pixmap);
  d->UI.image->setFixedSize(pixmap.size());

  // Hide unneeded "don't ask" controls
  d->UI.noAsk->setHidden(askKey.isEmpty());
  d->UI.noAskSession->hide();
  d->UI.noAskEver->hide();
}

//-----------------------------------------------------------------------------
qtConfirmationDialog::~qtConfirmationDialog()
{
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::setPrompt(const QString& text)
{
  QTE_D(qtConfirmationDialog);
  d->UI.label->setText(text);
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::setScope(AskPreferenceScope scope)
{
  QTE_D(qtConfirmationDialog);

  d->Scope = scope;

  switch (scope)
    {
    case qtConfirmationDialog::RememberSession:
      d->UI.noAskSession->setChecked(true);
      Q_FALLTHROUGH();
    case qtConfirmationDialog::RememberForever:
      d->UI.noAskEver->setChecked(true);
    default:
      break;
    }
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::setConfirmText(const QString& text)
{
  QTE_D(qtConfirmationDialog);
  d->UI.buttonBox->button(QDialogButtonBox::Ok)->setText(text);
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::updateLabelSize()
{
  QTE_D(qtConfirmationDialog);
  d->UI.label->setMinimumHeight(0);
  d->UI.label->setMinimumHeight(
    d->UI.label->heightForWidth(d->UI.label->width()));
  this->setMinimumSize(this->layout()->minimumSize());
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::updateNoAskPreference(bool noAsk)
{
  QTE_D(qtConfirmationDialog);

  // Enable or disable cancel (do not allow to cancel but also set 'no ask')
  d->UI.buttonBox->button(QDialogButtonBox::Cancel)->setEnabled(!noAsk);

  if (d->Scope == qtConfirmationDialog::UserCanChoose)
    {
    // Show/hide 'when to not ask' controls
    d->UI.noAskSession->setVisible(noAsk);
    d->UI.noAskEver->setVisible(noAsk);

    // Recalculate minimum size and update current size
    this->setMinimumSize(this->layout()->minimumSize());
    this->resize(this->size().expandedTo(this->minimumSize()));
    }
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::resizeEvent(QResizeEvent* e)
{
  QDialog::resizeEvent(e);
  this->updateLabelSize();
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::showEvent(QShowEvent* e)
{
  QDialog::showEvent(e);
  this->updateLabelSize();
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::accept()
{
  QTE_D(qtConfirmationDialog);
  bool ask = !d->UI.noAsk->isChecked();
  qtConfirmationDialog::setAsk(
    d->AskKey, ask, !ask && d->UI.noAskEver->isChecked());
  QDialog::accept();
}

//-----------------------------------------------------------------------------
bool qtConfirmationDialog::getConfirmation(
  QWidget* parent, const QString& askKey, const QString& prompt, QString title,
  QString confirmText, AskPreferenceScope scope, Qt::WindowFlags flags)
{
  return qtConfirmationDialog::getConfirmation(
    parent, askKey, prompt, confirmText, scope, title, flags);
}

//-----------------------------------------------------------------------------
bool qtConfirmationDialog::getConfirmation(
  QWidget* parent, const QString& askKey, const QString& prompt,
  const QString& confirmText, qtConfirmationDialog::AskPreferenceScope scope,
  QString title, Qt::WindowFlags flags)
{
  if (!qtConfirmationDialog::willAsk(askKey))
    {
    return true;
    }

  // Set up dialog
  QScopedPointer<qtConfirmationDialog>self(
    new qtConfirmationDialog(askKey, parent, flags));
  self->setWindowTitle(title.isEmpty() ? qApp->applicationName() : title);
  self->setPrompt(prompt);
  if (!confirmText.isEmpty())
    {
    self->setConfirmText(confirmText);
    }
  self->setScope(scope);

  // Execute dialog
  return (self->exec() == QDialog::Accepted);
}

//-----------------------------------------------------------------------------
bool qtConfirmationDialog::willAsk(const QString& key)
{
  if (key.isEmpty())
    {
    return true;
    }

  QVariantHash hash = qApp->property("ConfirmationDialogs").toHash();
  if (hash.contains(key))
    {
    return hash[key].toBool();
    }

  QSettings settings;
  settings.beginGroup("ConfirmationDialogs");
  return settings.value(key, true).toBool();
}

//-----------------------------------------------------------------------------
void qtConfirmationDialog::setAsk(
  const QString& key, bool ask, bool persistent)
{
  if (key.isEmpty())
    {
    return;
    }

  QVariantHash hash = qApp->property("ConfirmationDialogs").toHash();
  hash.insert(key, ask);
  qApp->setProperty("ConfirmationDialogs", hash);

  if (persistent)
    {
    QSettings settings;
    settings.beginGroup("ConfirmationDialogs");
    settings.setValue(key, ask);
    }
}
