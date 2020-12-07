// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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

    qtSqueezedLabel(QWidget* parent = nullptr, Qt::WindowFlags = {});
    qtSqueezedLabel(QString const& text, QWidget* parent = nullptr,
                    Qt::WindowFlags = {});
    virtual ~qtSqueezedLabel();

    ElideMode elideMode() const;
    void setElideMode(ElideMode);

    QString fullText() const;

    using QLabel::setText;
    void setText(QString const&, SetTextMode);

    using QLabel::setToolTip;
    void setToolTip(QString const&, SetToolTipMode);

    virtual QSize minimumSizeHint() const;

    void setTextMargins(qreal leftMargin, qreal rightMargin);

public slots:
    void setFullText(QString const&);
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
    QTE_DISABLE_COPY(qtSqueezedLabel)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(qtSqueezedLabel::ElideMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(qtSqueezedLabel::SetTextMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(qtSqueezedLabel::SetToolTipMode)

#endif
