/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtActionManagerDialog_h
#define __qtActionManagerDialog_h

#include <QDialog>
#include <QString>

#include "../core/qtGlobal.h"

class qtActionManagerDialogPrivate;

class QTE_EXPORT qtActionManagerDialog : public QDialog
{
  Q_OBJECT

public:
  qtActionManagerDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
  virtual ~qtActionManagerDialog();

protected slots:
  void filterChanged(QString text = QString());

protected:
  QTE_DECLARE_PRIVATE_PTR(qtActionManagerDialog)
  friend class qtActionFactory;

  void registerAction(QString key, QString displayGroup, QString text,
                      QString icon, QKeySequence defaultShortcut);
  void registerAction(QString key, QString displayGroup, QString text,
                      QString icon, QList<QKeySequence> defaultShortcuts);

private:
  QTE_DECLARE_PRIVATE(qtActionManagerDialog)
  Q_DISABLE_COPY(qtActionManagerDialog)
};

#endif
