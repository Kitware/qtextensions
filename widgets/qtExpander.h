/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtExpander_h
#define __qtExpander_h

#include <QToolButton>

#include "../core/qtGlobal.h"

class QTE_EXPORT qtExpander : public QToolButton
{
  Q_OBJECT

public:
  qtExpander(QWidget* parent = 0);
  qtExpander(bool initiallyExpanded, QWidget* parent = 0);

  QSize sizeHint() const;
  QSize minimumSizeHint() const;
  using QWidget::size;

  static QSize size(QStyle* style,
                    QString text = QString(),
                    QFont font = QFont());

protected:
  static QSize arrowSize();
  static int margin(QStyle*, bool hasText);

  void init();
  void paintEvent(QPaintEvent*);

private:
  QTE_DISABLE_COPY(qtExpander)
};

#endif
