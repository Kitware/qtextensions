// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtEditableLabel_h
#define __qtEditableLabel_h

#include <QLabel>

#include "../core/qtGlobal.h"

class qtEditableLabelPrivate;

class qtEditableLabel : public QLabel
{
  Q_OBJECT

public:
  qtEditableLabel(QWidget* parent = 0);
  virtual ~qtEditableLabel();

public slots:
  void beginEditing();
  void endEditing();
  void cancelEditing();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtEditableLabel)

  void mouseDoubleClickEvent(QMouseEvent* event);

private:
  QTE_DECLARE_PRIVATE(qtEditableLabel)
  QTE_DISABLE_COPY(qtEditableLabel)
};

#endif
