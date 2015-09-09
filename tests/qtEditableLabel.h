/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
  Q_DISABLE_COPY(qtEditableLabel)
};

#endif
