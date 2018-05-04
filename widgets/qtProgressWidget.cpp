/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtProgressWidget.h"

// Qt includes
#include <QDebug>
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
    : q_ptr{ q }
  {
    this->autoHide = true;
    this->nameVisible = false;
    this->descriptionVisible = true;
  }
  bool autoHide;
  bool nameVisible;
  bool descriptionVisible;
  QHash<QString, QWidget*> progressBars;

  void updateVisibility();
  void updateNameDescriptionVisibility();
  void setValue(const QString& name, int value);
  void setRange(const QString& name, int minimum, int maximum);
  void setDescription(const QString& name, const QString& description);
  QWidget* addProgressBar(const QString& name,
                          const QString& description,
                          int value);
  void removeProgressBar(const QString& name);
  // Get access to the internal widget's name label
  QLabel* nameLabel(QWidget*);
  // Get access to the internal widget's description label
  QLabel* descriptionLabel(QWidget*);
  // Get access to the internal widget's progress bar
  QProgressBar* progressBar(QWidget*);
  bool checkName(const QString& name);

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
void qtProgressWidgetPrivate::setDescription(const QString& name,
                                             const QString& description)
{
  QWidget* widget = this->progressBars.value(name, nullptr);

  // Check if we already have a progress bar for the task.
  if (widget)
  {
    // Update the text description
    this->descriptionLabel(widget)->setText(description);
  }
  else
  {
    this->addProgressBar(name, description, 0);
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::setRange(const QString& name,
                                       int minimum,
                                       int maximum)
{
  QWidget* widget = this->progressBars.value(name, nullptr);
  if (widget)
  {
    // Update the range
    this->progressBar(widget)->setRange(minimum, maximum);
  }
  else
  {
    widget = this->addProgressBar(name, "", 0);
    // Update the range
    this->progressBar(widget)->setRange(minimum, maximum);
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateNameDescriptionVisibility()
{
  QHash<QString, QWidget*>::const_iterator progressBarsIter =
    this->progressBars.begin();
  for (progressBarsIter = this->progressBars.constBegin();
       progressBarsIter != this->progressBars.constEnd();
       progressBarsIter++)
  {
    QWidget* widget = progressBarsIter.value();
    QLabel* nameLabel = this->nameLabel(widget);
    nameLabel->setVisible(this->nameVisible);
    QLabel* descriptionLabel = this->descriptionLabel(widget);
    descriptionLabel->setVisible(this->descriptionVisible);
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::setValue(const QString& name, int value)
{
  QWidget* widget = this->progressBars.value(name, nullptr);

  // Check if we already have a progress bar for the task.
  if (widget)
  {
    // Update the progress value
    this->progressBar(widget)->setValue(value);
  }
  else
  {
    this->addProgressBar(name, "", value);
  }
}

//-----------------------------------------------------------------------------
QWidget* qtProgressWidgetPrivate::addProgressBar(const QString& name,
                                                 const QString& description,
                                                 int value)
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
  nameLabel->setVisible(this->nameVisible);
  QLabel* descriptionLabel = new QLabel(widget);
  descriptionLabel->setText(description);
  hbox->addWidget(descriptionLabel, 5, Qt::AlignJustify);
  descriptionLabel->setVisible(this->descriptionVisible);

  QProgressBar* bar = new QProgressBar(widget);
  vbox->addWidget(bar);
  bar->setValue(value);
  widget->setObjectName(name);

  q->layout()->addWidget(widget);
  this->progressBars.insert(name, widget);

  this->updateVisibility();
  return widget;
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::removeProgressBar(const QString& name)
{
  QTE_Q(qtProgressWidget);

  QWidget* widget = this->progressBars.value(name, nullptr);

  if (widget)
  {
    q->layout()->removeWidget(widget);
    this->progressBars.remove(name);
    delete widget;

    this->updateVisibility();
  }
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
QProgressBar* qtProgressWidgetPrivate::progressBar(QWidget* widget)
{
  if (!widget)
  {
    return nullptr;
  }
  return qobject_cast<QProgressBar*>(widget->layout()->itemAt(1)->widget());
}

//-----------------------------------------------------------------------------
bool qtProgressWidgetPrivate::checkName(const QString& name)
{
  bool empty = name.isEmpty();
  if (empty)
  {
    qCritical() << "qtProgressWidget: name cannot be empty";
  }
  return !empty;
}

//-----------------------------------------------------------------------------
qtProgressWidget::qtProgressWidget(QWidget* parent)
  : QWidget(parent)
  , d_ptr{ new qtProgressWidgetPrivate(this) }
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
bool qtProgressWidget::autoHide() const
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
bool qtProgressWidget::nameVisible() const
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
    d->updateNameDescriptionVisibility();
  }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::descriptionVisible() const
{
  QTE_D();
  return d->descriptionVisible;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setDescriptionVisible(bool visible)
{
  QTE_D();
  if (d->descriptionVisible != visible)
  {
    d->descriptionVisible = visible;
    d->updateNameDescriptionVisibility();
  }
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setDescription(const QString& name,
                                      const QString& description)
{
  QTE_D();
  if (!d->checkName(name))
  {
    return;
  }
  d->setDescription(name, description);
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setRange(const QString& name, int minimum, int maximum)
{
  QTE_D();
  if (!d->checkName(name))
  {
    return;
  }
  d->setRange(name, minimum, maximum);
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setValue(const QString& name, int value)
{
  QTE_D();
  if (!d->checkName(name))
  {
    return;
  }
  d->setValue(name, value);
}

//-----------------------------------------------------------------------------
void qtProgressWidget::remove(const QString& name)
{
  QTE_D();
  if (!d->checkName(name))
  {
    return;
  }
  d->removeProgressBar(name);
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::addProgressBar(const QString& name,
                                      const QString& description,
                                      int value,
                                      int minimum,
                                      int maximum)
{
  QTE_D();
  if (!d->checkName(name))
  {
    return false;
  }
  d->addProgressBar(name, description, value);
  d->setRange(name, minimum, maximum);
  return true;
}

//-----------------------------------------------------------------------------
QHash<QString, QWidget*> qtProgressWidget::progressBars()
{
  QTE_D();
  return d->progressBars;
}
