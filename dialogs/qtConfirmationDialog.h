// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtConfirmationDialog_h
#define __qtConfirmationDialog_h

#include <QDialog>

#include "../core/qtGlobal.h"

class qtConfirmationDialogPrivate;

class QTE_EXPORT qtConfirmationDialog : public QDialog
{
  Q_OBJECT

public:
  enum AskPreferenceScope
    {
    RememberSession,
    RememberForever,
    UserCanChoose
    };

  qtConfirmationDialog(const QString& askKey, QWidget* parent = nullptr,
                       Qt::WindowFlags flags = {});
  virtual ~qtConfirmationDialog();

  void setPrompt(const QString&);
  void setScope(AskPreferenceScope);
  void setConfirmText(const QString&);

  virtual void accept();

  virtual void resizeEvent(QResizeEvent*);
  virtual void showEvent(QShowEvent*);

  static bool getConfirmation(
    QWidget* parent, const QString& askKey, const QString& prompt,
    const QString& confirmText, AskPreferenceScope scope,
    QString title = {}, Qt::WindowFlags flags = {});
  static bool getConfirmation(
    QWidget* parent, const QString& askKey, const QString& prompt,
    QString title = {}, QString confirmText = {},
    AskPreferenceScope scope = UserCanChoose, Qt::WindowFlags flags = {});

  static bool willAsk(const QString& key);
  static void setAsk(const QString& key, bool ask, bool persistent);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtConfirmationDialog)

  void updateLabelSize();

protected slots:
  void updateNoAskPreference(bool);

private:
  QTE_DECLARE_PRIVATE(qtConfirmationDialog)
  QTE_DISABLE_COPY(qtConfirmationDialog)
};

#endif
