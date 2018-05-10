/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtProgressWidget.h"

#include "../core/qtGet.h"

#include <QHash>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

QTE_IMPLEMENT_D_FUNC(qtProgressWidget)

//-----------------------------------------------------------------------------
class qtProgressWidgetPrivate
{
public:
    struct Task
    {
        QWidget* container;
        QLabel* label;
        QProgressBar* progressBar;
    };

    bool autoHide = true;
    bool labelVisible = true;

    QHash<int, Task> tasks;
    QList<int> availableIds;
    int lastTaskId = -1;

    void updateVisibility(qtProgressWidget* self);
    void updateLabelVisibility();

    int newTaskId();
};

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateVisibility(qtProgressWidget* self)
{
  self->setVisible(!this->autoHide || !this->tasks.empty());
}

//-----------------------------------------------------------------------------
void qtProgressWidgetPrivate::updateLabelVisibility()
{
  foreach (auto const& task, this->tasks)
    task.label->setVisible(this->labelVisible);
}

//-----------------------------------------------------------------------------
int qtProgressWidgetPrivate::newTaskId()
{
    if (!this->availableIds.isEmpty())
        return this->availableIds.takeLast();
    return ++this->lastTaskId;
}

//-----------------------------------------------------------------------------
qtProgressWidget::qtProgressWidget(QWidget* parent) :
    QWidget{parent}, d_ptr{new qtProgressWidgetPrivate}
{
    this->setLayout(new QVBoxLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    // Because autoHide is initially true, and we have no tasks yet, we should
    // start out hidden
    this->hide();
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
        d->updateVisibility(this);
    }
}

//-----------------------------------------------------------------------------
bool qtProgressWidget::labelVisible() const
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
        d->updateLabelVisibility();
    }
}

//-----------------------------------------------------------------------------
int qtProgressWidget::addTask(QString const& text, int value,
                              int minimum, int maximum)
{
    QTE_D();

    QWidget* container = new QWidget{this};
    container->setLayout(new QVBoxLayout);
    container->layout()->setSpacing(0);
    container->layout()->setContentsMargins(0, 0, 0, 0);

    qtProgressWidgetPrivate::Task task{
        container,
        new QLabel{text, container},
        new QProgressBar{container},
    };

    container->layout()->addWidget(task.label);
    container->layout()->addWidget(task.progressBar);
    this->layout()->addWidget(container);

    task.label->setVisible(d->labelVisible);

    task.progressBar->setRange(minimum, maximum);
    task.progressBar->setValue(value);

    auto const id = d->newTaskId();
    d->tasks.insert(id, task);

    d->updateVisibility(this);

    return id;
}

//-----------------------------------------------------------------------------
void qtProgressWidget::removeTask(int id)
{
    QTE_D();

    auto const iter = d->tasks.find(id);
    if (iter != d->tasks.end())
    {
        delete iter->container;
        d->tasks.erase(iter);
        d->availableIds.append(id);
        d->updateVisibility(this);
    }
}

//-----------------------------------------------------------------------------
QList<int> qtProgressWidget::tasks() const
{
    QTE_D();
    return d->tasks.keys();
}

//-----------------------------------------------------------------------------
QString qtProgressWidget::taskText(int id) const
{
    QTE_D();
    if (auto const* const t = qtGet(d->tasks, id))
        return t->label->text();

    return {};
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setTaskText(int id, QString const& text)
{
    QTE_D();
    if (auto const* const t = qtGet(d->tasks, id))
        t->label->setText(text);
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setProgressRange(int id, int minimum, int maximum)
{
    QTE_D();
    if (auto const* const t = qtGet(d->tasks, id))
        t->progressBar->setRange(minimum, maximum);
}

//-----------------------------------------------------------------------------
void qtProgressWidget::setProgressValue(int id, int value)
{
    QTE_D();
    if (auto const* const t = qtGet(d->tasks, id))
        t->progressBar->setValue(value);
}
