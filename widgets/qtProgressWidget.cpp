/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

// QTE includes
#include "qtProgressWidget.h"

// Qt includes
#include <QHBoxLayout>
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
  QVector<QPointer<QWidget>> ProgressBars;

  void autoHideWidget();
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
void qtProgressWidgetPrivate::autoHideWidget()
{
  QTE_Q(qtProgressWidget);
  if (this->autoHide)
  {
    if (this->ProgressBars.empty())
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
  QPointer<QWidget> widget = nullptr;

  for (int i = 0; i < this->ProgressBars.size(); ++i)
  {
    widget = this->ProgressBars.at(i);
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
      this->ProgressBars.erase(this->ProgressBars.begin() + i);
      delete widget;
    }
    else if (v < 1 && this->busyOnZero)
    {
      bar->setMinimum(0);
      bar->setMaximum(0);
      bar->setValue(v);
    }
    else
    {
      bar->setMaximum(100);
      bar->setValue(v);
    }
  }

  this->autoHideWidget();
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateProgress(QString& name,
                                             int value,
                                             QString& text)
{
  QPointer<QWidget> widget = nullptr;

  // Check if we already have a progress bar for the task.
  for (int i = 0; i < this->ProgressBars.size(); ++i)
  {
    widget = this->ProgressBars.at(i);
    if (widget && (widget->objectName().compare(name) == 0))
    {
      widget->setProperty("value", value);
      // Update the text description
      this->descriptionLabel(widget)->setText(text);
      break;
    }
    else
    {
      widget = nullptr;
    }
  }

  if (!widget)
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
  this->ProgressBars.push_back(widget);
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
  : Superclass(parent)
  , d_ptr(new qtProgressWidgetPrivate(this))
{
  QTE_D(qtProgressWidget);
  QVBoxLayout* layout = new QVBoxLayout();
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);

  d->autoHideWidget();
}

//-----------------------------------------------------------------------------
qtProgressWidget::~qtProgressWidget()
{
  QTE_D(qtProgressWidget);
  d->ProgressBars.clear();
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::autoHide()
{
  QTE_D_CONST(qtProgressWidget);
  return d->autoHide;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setAutoHide(bool hide)
{
  QTE_D(qtProgressWidget);
  if (d->autoHide != hide)
  {
    d->autoHide = hide;
    d->autoHideWidget();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::busyOnZero()
{
  QTE_D_CONST(qtProgressWidget);
  return d->busyOnZero;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setBusyOnZero(bool busy)
{
  QTE_D(qtProgressWidget);
  if (d->busyOnZero != busy)
  {
    d->busyOnZero = busy;
    d->updateProgressBars();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::nameVisible()
{
  QTE_D_CONST(qtProgressWidget);
  return d->nameVisible;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setNameVisible(bool visible)
{
  QTE_D(qtProgressWidget);
  if (d->nameVisible != visible)
  {
    d->nameVisible = visible;
    d->updateProgressBars();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::labelVisible()
{
  QTE_D_CONST(qtProgressWidget);
  return d->labelVisible;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setLabelVisible(bool visible)
{
  QTE_D(qtProgressWidget);
  if (d->labelVisible != visible)
  {
    d->labelVisible = visible;
    d->updateProgressBars();
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidget::updateProgress(QString name, int value, QString text)
{
  QTE_D(qtProgressWidget);
  d->updateProgress(name, value, text);
}
