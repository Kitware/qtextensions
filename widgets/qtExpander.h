// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtExpander_h
#define __qtExpander_h

#include <QToolButton>

#include "../core/qtGlobal.h"

class QTE_EXPORT qtExpander : public QToolButton
{
  Q_OBJECT

public:
  qtExpander(QWidget* parent = nullptr);
  qtExpander(bool initiallyExpanded, QWidget* parent = nullptr);

  QSize sizeHint() const;
  QSize minimumSizeHint() const;
  using QWidget::size;

  static QSize size(QStyle* style, QString text = {}, QFont font = {});

protected:
  static QSize arrowSize();
  static int margin(QStyle*, bool hasText);

  void init();
  void paintEvent(QPaintEvent*);

private:
  QTE_DISABLE_COPY(qtExpander)
};

#endif
