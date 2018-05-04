/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtProgressWidget_h
#define __qtProgressWidget_h

#include "../core/qtGlobal.h"

// Qt includes
#include <QWidget>

// Forward declarations
class qtProgressWidgetPrivate;

/// Custom widget that shows and manages multiple progress bars and associated
/// status messages / task descriptions.
class QTE_EXPORT qtProgressWidget : public QWidget
{
  Q_OBJECT

  /// This property controls whether the widget will be hidden automatically
  /// when there are no active progress bars.
  ///
  /// By default, this property is \c true.
  Q_PROPERTY(bool autoHide READ autoHide WRITE setAutoHide)

  /// This property controls whether the task name should be visible.
  /// If true, the task name is displayed as <tt>name| description</tt> above
  /// the progress bar.
  ///
  /// By default, this property is \c false.
  ///
  /// \sa labelVisible
  Q_PROPERTY(bool nameVisible READ nameVisible WRITE setNameVisible)

  /// This property controls whether the description label should be visible.
  /// If true, the label is displayed above the progress bar.
  ///
  /// By default, this property is \c true.
  ///
  /// \sa nameVisible
  Q_PROPERTY(
    bool descriptionVisible READ descriptionVisible WRITE setDescriptionVisible)

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

  /// Add a progress bar to the widget.
  ///
  /// \return Boolean indicating whether the progress bar was successfully
  /// added to the widget.
  virtual bool addProgressBar(const QString& name,
                              const QString& description = QString(""),
                              int value = 0,
                              int minimum = 0,
                              int maximum = 100);

  /// Set the description of a particular progress bar.
  ///
  /// \note This method adds a new progress bar if none match the \c name key.
  virtual void setDescription(const QString& name, const QString& description);

  /// Set the value range for a particular progress bar.
  ///
  /// This method can be useful to set custom ranges for different progress bars
  /// managed by the qtProgressWidget. If the minimum and maximum are set to the
  /// same value, the progress bar enters a special busy state.
  ///
  /// \note This method adds a new progress bar if none match the \c name key.
  ///
  /// \sa setDescription, setValue
  virtual void setRange(const QString& name, int minimum, int maximum);

  /// Set the value of a particular progress bar.
  /// The progress bar is identified by the \c name parameter. If no progress
  /// with that name exists, this method adds one.
  ///
  /// \note This method adds a new progress bar if none match the \c name key.
  virtual void setValue(const QString& name, int value);

  /// Remove a particular progress bar from the widget
  virtual void remove(const QString& name);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtProgressWidget)

private:
  QTE_DECLARE_PRIVATE(qtProgressWidget)
  QTE_DISABLE_COPY(qtProgressWidget)
};

#endif
