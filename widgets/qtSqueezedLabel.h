/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtSqueezedLabel_h
#define __qtSqueezedLabel_h

#include <QLabel>

#include "../core/qtGlobal.h"

class qtSqueezedLabelPrivate;

class QTE_EXPORT qtSqueezedLabel : public QLabel
{
  Q_OBJECT

  Q_FLAGS(ElideMode)

  Q_PROPERTY(ElideMode elideMode READ elideMode WRITE setElideMode)
  Q_PROPERTY(QString fullText READ fullText WRITE setFullText)

public:
  enum ElideFlag
    {
    // ElideMiddle   = 0x0,
    // ElideStart    = 0x1,
    ElideEnd      = 0x2,
    ElideEllipsis = 0x0,
    ElideFade     = 0x8
    };
  Q_DECLARE_FLAGS(ElideMode, ElideFlag)

  enum SetTextFlag
    {
    SetFullText = 0x1,
    SetToolTip = 0x2,
    SimplifyText = 0x8
    };
  Q_DECLARE_FLAGS(SetTextMode, SetTextFlag)

  enum SetToolTipFlag
    {
    PlainText = 0x1,
    AutoWrap = 0x2
    };
  Q_DECLARE_FLAGS(SetToolTipMode, SetToolTipFlag)

  qtSqueezedLabel(QWidget* parent = 0, Qt::WindowFlags = 0);
  qtSqueezedLabel(const QString& text, QWidget* parent = 0,
                  Qt::WindowFlags = 0);
  virtual ~qtSqueezedLabel();

  ElideMode elideMode() const;
  void setElideMode(ElideMode);

  QString fullText() const;

  using QLabel::setText;
  void setText(const QString&, SetTextMode);

  using QLabel::setToolTip;
  void setToolTip(const QString&, SetToolTipMode);

  virtual QSize minimumSizeHint() const;

public slots:
  void setFullText(const QString&);
  void copy();

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtSqueezedLabel)

  virtual bool event(QEvent*);
  virtual void changeEvent(QEvent*);
  virtual void contextMenuEvent(QContextMenuEvent*);
  virtual void moveEvent(QMoveEvent*);
  virtual void resizeEvent(QResizeEvent*);
  virtual void paintEvent(QPaintEvent*);

private:
  QTE_DECLARE_PRIVATE(qtSqueezedLabel)
  Q_DISABLE_COPY(qtSqueezedLabel)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(qtSqueezedLabel::ElideMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(qtSqueezedLabel::SetTextMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(qtSqueezedLabel::SetToolTipMode)

#endif
