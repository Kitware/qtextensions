/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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

  qtConfirmationDialog(
    const QString& askKey, QWidget* parent = 0, Qt::WindowFlags flags = 0);
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
    QString title = QString(), Qt::WindowFlags flags = 0);
  static bool getConfirmation(
    QWidget* parent, const QString& askKey, const QString& prompt,
    QString title = QString(), QString confirmText = QString(),
    AskPreferenceScope scope = UserCanChoose, Qt::WindowFlags flags = 0);

  static bool willAsk(const QString& key);
  static void setAsk(const QString& key, bool ask, bool persistent);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtConfirmationDialog)

  void updateLabelSize();

protected slots:
  void updateNoAskPreference(bool);

private:
  QTE_DECLARE_PRIVATE(qtConfirmationDialog)
  Q_DISABLE_COPY(qtConfirmationDialog)
};

#endif
