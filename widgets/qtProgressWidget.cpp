/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

// QTE includes
#include "qtProgressWidget.h"

// Qt includes
#include <QHBoxLayout>
#include <QHash>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QVBoxLayout>
#include <QVariant>

QTE_IMPLEMENT_D_FUNC(qtProgressWidget)

//-----------------------------------------------------------------------------
class qtProgressWidgetPrivate
{
public:
  explicit qtProgressWidgetPrivate(qtProgressWidget* q)
    : q_ptr(q)
  {
    this->autoHide = true;
    this->busyOnZero = false;
    this->nameVisible = false;
    this->labelVisible = true;
  }
  bool autoHide;
  bool busyOnZero;
  bool nameVisible;
  bool labelVisible;
  QHash<QString, QWidget*> progressBars;

  void updateVisibility();
  void updateProgressBars();
  void updateProgress(QString& name, int value, QString& text);
  void addProgressBar(QString& name, int value, QString& label);
  QLabel* nameLabel(QWidget*);
  QLabel* descriptionLabel(QWidget*);

protected:
  QTE_DECLARE_PUBLIC_PTR(qtProgressWidget)

private:
  QTE_DECLARE_PUBLIC(qtProgressWidget)
};

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateVisibility()
{
  QTE_Q(qtProgressWidget);
  if (this->autoHide)
  {
    if (this->progressBars.empty())
    {
      q->hide();
    }
    else
    {
      q->show();
    }
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateProgressBars()
{
  QTE_Q(qtProgressWidget);

  QHash<QString, QWidget*>::iterator progressBarsIter =
    this->progressBars.begin();
  while (progressBarsIter != this->progressBars.end())
  {
    QWidget* widget = progressBarsIter.value();
    QLabel* nameLabel = this->nameLabel(widget);
    nameLabel->setVisible(this->nameVisible);
    QLabel* descriptionLabel = this->descriptionLabel(widget);
    descriptionLabel->setVisible(this->labelVisible);
    QProgressBar* bar =
      qobject_cast<QProgressBar*>(widget->layout()->itemAt(1)->widget());
    int v = widget->property("value").toInt();
    if (v >= 99)
    {
      // Remove the progress bar along with its label
      q->layout()->removeWidget(widget);
      progressBarsIter = this->progressBars.erase(progressBarsIter);
      delete widget;
    }
    else if (v < 1 && this->busyOnZero)
    {
      bar->setMinimum(0);
      bar->setMaximum(0);
      bar->setValue(v);
      progressBarsIter++;
    }
    else
    {
      bar->setMaximum(100);
      bar->setValue(v);
      progressBarsIter++;
    }
  }

  this->updateVisibility();
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateProgress(QString& name,
                                             int value,
                                             QString& text)
{
  QWidget* widget = this->progressBars.value(name, nullptr);

  // Check if we already have a progress bar for the task.
  if (widget)
  {
    widget->setProperty("value", value);
    // Update the text description
    this->descriptionLabel(widget)->setText(text);
  }
  else
  {
    this->addProgressBar(name, value, text);
  }

  this->updateProgressBars();
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::addProgressBar(QString& name,
                                             int value,
                                             QString& text)
{
  QTE_Q(qtProgressWidget);
  QWidget* widget = new QWidget(q);
  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setSpacing(0);
  vbox->setContentsMargins(0, 0, 0, 0);
  widget->setLayout(vbox);
  QHBoxLayout* hbox = new QHBoxLayout();
  vbox->addLayout(hbox);
  QLabel* nameLabel = new QLabel(widget);
  nameLabel->setText(name);
  nameLabel->setStyleSheet("font-weight: bold;");
  hbox->addWidget(nameLabel, 0, Qt::AlignJustify);
  QLabel* descriptionLabel = new QLabel(widget);
  descriptionLabel->setText(text);
  hbox->addWidget(descriptionLabel, 5, Qt::AlignJustify);

  QProgressBar* bar = new QProgressBar(widget);
  vbox->addWidget(bar);
  bar->setValue(value);
  widget->setObjectName(name);
  widget->setProperty("value", value);

  q->layout()->addWidget(widget);
  this->progressBars.insert(name, widget);
}

//-----------------------------------------------------------------------------
QLabel* qtProgressWidgetPrivate::nameLabel(QWidget* widget)
{
  if (!widget)
  {
    return nullptr;
  }
  return qobject_cast<QLabel*>(
    widget->layout()->itemAt(0)->layout()->itemAt(0)->widget());
}

//-----------------------------------------------------------------------------
QLabel* qtProgressWidgetPrivate::descriptionLabel(QWidget* widget)
{
  if (!widget)
  {
    return nullptr;
  }
  return qobject_cast<QLabel*>(
    widget->layout()->itemAt(0)->layout()->itemAt(1)->widget());
}

//-----------------------------------------------------------------------------
qtProgressWidget::qtProgressWidget(QWidget* parent)
  : Superclass(parent),
    d_ptr(new qtProgressWidgetPrivate(this))
{
  QTE_D(qtProgressWidget);
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);

  d->updateVisibility();
}

//-----------------------------------------------------------------------------
qtProgressWidget::~qtProgressWidget()
{
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::autoHide()
{
  QTE_D();
  return d->autoHide;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setAutoHide(bool hide)
{
  QTE_D();
  if (d->autoHide != hide)
  {
    d->autoHide = hide;
    d->updateVisibility();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::busyOnZero()
{
  QTE_D();
  return d->busyOnZero;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setBusyOnZero(bool busy)
{
  QTE_D();
  if (d->busyOnZero != busy)
  {
    d->busyOnZero = busy;
    d->updateProgressBars();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::nameVisible()
{
  QTE_D();
  return d->nameVisible;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setNameVisible(bool visible)
{
  QTE_D();
  if (d->nameVisible != visible)
  {
    d->nameVisible = visible;
    d->updateProgressBars();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::labelVisible()
{
  QTE_D();
  return d->labelVisible;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setLabelVisible(bool visible)
{
  QTE_D();
  if (d->labelVisible != visible)
  {
    d->labelVisible = visible;
    d->updateProgressBars();
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidget::updateProgress(QString name, int value, QString text)
{
  QTE_D();
  d->updateProgress(name, value, text);
}
