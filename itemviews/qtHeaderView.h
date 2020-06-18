/*ckwg +5
 * Copyright 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtHeaderView_h
#define __qtHeaderView_h

#include <QHeaderView>

#include "../core/qtGlobal.h"

class qtHeaderViewPrivate;

class QTE_EXPORT qtHeaderView : public QHeaderView
{
  Q_OBJECT

public:
  qtHeaderView(Qt::Orientation orientation, QWidget* parent = 0);
  virtual ~qtHeaderView();

  bool restoreState(const QByteArray &state);
  QByteArray saveState() const;

protected:
  QTE_DECLARE_PRIVATE(qtHeaderView)

private slots:
  void customContextMenu(const QPoint &pos);

private:
  QTE_DISABLE_COPY(qtHeaderView)
  QTE_DECLARE_PRIVATE_RPTR(qtHeaderView)
};

#endif
