// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtProgressWidget_h
#define __qtProgressWidget_h

#include "../core/qtGlobal.h"

#include <QWidget>

class qtProgressWidgetPrivate;

/// Custom widget that shows and manages multiple tasks. A task is a progress
/// bar with an associated name and an optional description.
class QTE_EXPORT qtProgressWidget : public QWidget
{
    Q_OBJECT

    /// This property controls whether the widget will be hidden automatically
    /// when there are no active tasks.
    ///
    /// By default, this property is \c true.
    Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide)

    /// This property controls whether the task label should be visible. If
    /// true, the task label is displayed above the progress bar.
    ///
    /// By default, this property is \c true.
    Q_PROPERTY(bool labelVisible READ labelVisible WRITE setLabelVisible)

    /// This property controls the text alignment of the task labels.
    Q_PROPERTY(Qt::Alignment labelAlignment READ labelAlignment
                                            WRITE setLabelAlignment)

public:
    qtProgressWidget(QWidget* parent = nullptr);
    virtual ~qtProgressWidget();

    bool autoHide() const;
    bool labelVisible() const;
    Qt::Alignment labelAlignment() const;

    // Get list of all tasks.
    QList<int> tasks() const;

    // Get the text of the specified task.
    QString taskText(int id) const;

public slots:
    void setAutoHide(bool hide);
    void setLabelVisible(bool visible);
    void setLabelAlignment(Qt::Alignment);

    /// Add a task to the widget.
    ///
    /// \return Identifier of the newly added task.
    virtual int addTask(QString const& text = QString(""), int value = 0,
                        int minimum = 0, int maximum = 100);

    // Set the text of a particular task.
    void setTaskText(int id, QString const& text);

    /// Set the text alignment of the task labels.

    /// Set the value range for a particular task's progress bar.
    ///
    /// This method can be useful to set custom ranges for different progress
    /// bars managed by the qtProgressWidget. If the minimum and maximum are
    /// set to the same value, the progress bar enters a special busy state.
    ///
    /// \sa setValue, QProgressBar::setRange
    void setProgressRange(int id, int minimum, int maximum);

    /// Set the value of a particular task's progress bar.
    ///
    /// \sa setValue, QProgressBar::setValue
    void setProgressValue(int id, int value);

    /// Remove a particular task from the widget.
    void removeTask(int id);

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtProgressWidget)

private:
    QTE_DECLARE_PRIVATE(qtProgressWidget)
    QTE_DISABLE_COPY(qtProgressWidget)
};

#endif
