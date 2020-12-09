// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
  qtActionManagerDialog(QWidget* parent = nullptr,
                        Qt::WindowFlags flags = {});
  virtual ~qtActionManagerDialog();

protected slots:
  void filterChanged(QString text = {});

protected:
  QTE_DECLARE_PRIVATE_PTR(qtActionManagerDialog)
  friend class qtActionFactory;

  void registerAction(QString key, QString displayGroup, QString text,
                      QString icon, QKeySequence defaultShortcut);
  void registerAction(QString key, QString displayGroup, QString text,
                      QString icon, QList<QKeySequence> defaultShortcuts);

private:
  QTE_DECLARE_PRIVATE(qtActionManagerDialog)
  QTE_DISABLE_COPY(qtActionManagerDialog)
};

#endif
