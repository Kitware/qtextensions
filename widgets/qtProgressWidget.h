/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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

    /// This property controls whether the task name should be visible.
    /// If true, the task name is displayed above the progress bar.
    ///
    /// By default, this property is \c false.
    ///
    /// \sa labelVisible
    Q_PROPERTY(bool nameVisible READ nameVisible WRITE setNameVisible)

    /// This property controls whether the description label should be visible.
    /// If true, the label is displayed above the progress bar and below the
    /// task name.
    ///
    /// By default, this property is \c true.
    ///
    /// \sa nameVisible
    Q_PROPERTY(bool descriptionVisible
               READ descriptionVisible
               WRITE setDescriptionVisible)

public:
    qtProgressWidget(QWidget* parent = nullptr);
    virtual ~qtProgressWidget();

    bool autoHide() const;
    bool nameVisible() const;
    bool descriptionVisible() const;

public slots:
    void setAutoHide(bool hide);
    void setNameVisible(bool visible);
    void setDescriptionVisible(bool visible);

    /// Add a task to the widget.
    ///
    /// \return Identifier of the newly added task.
    virtual int addTask(QString const& name,
                        QString const& description = QString(""),
                        int value = 0, int minimum = 0, int maximum = 100);

    // Get list of all tasks.
    QList<int> tasks() const;

    // Get the name of the specified task.
    QString taskName(int id) const;

    // Set the name of a particular task.
    void setTaskName(int id, QString const& name);

    /// Get the description of the specified task.
    QString taskDescription(int id) const;

    /// Set the description of a particular task.
    void setTaskDescription(int id, QString const& description);

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
