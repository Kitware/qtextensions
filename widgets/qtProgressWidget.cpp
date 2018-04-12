/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

// QTE includes
#include "qtProgressWidget.h"

// Qt includes
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
  }
  bool autoHide;
  bool busyOnZero;
  QVector<QPointer<QWidget>> ProgressBars;

  void autoHideWidget();
  void updateProgressBars();

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
    QLabel* label =
      qobject_cast<QLabel*>(widget->layout()->itemAt(0)->widget());
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
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidget::updateProgress(int value, QString text)
{
  QTE_D(qtProgressWidget);
  QPointer<QWidget> widget = nullptr;

  // Check if we already have a progress bar for the task.
  for (int i = 0; i < d->ProgressBars.size(); ++i)
  {
    widget = d->ProgressBars.at(i);
    QLabel* label =
      qobject_cast<QLabel*>(widget->layout()->itemAt(0)->widget());
    if (label && (label->text().compare(text) == 0))
    {
      widget->setProperty("value", value);
      break;
    }
    else
    {
      widget = nullptr;
    }
  }

  if (!widget)
  {
    widget = new QWidget(this);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->setSpacing(0);
    vbox->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(vbox);
    QLabel* label = new QLabel(widget);
    vbox->addWidget(label);
    label->setText(text);
    QProgressBar* bar = new QProgressBar(this);
    vbox->addWidget(bar);
    bar->setValue(value);
    widget->setProperty("value", value);
    this->layout()->addWidget(widget);
    d->ProgressBars.push_back(widget);
  }

  d->updateProgressBars();
}
