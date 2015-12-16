/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qtDrawer.h"

#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QStyle>

#include "../core/qtGlobal.h"

#include "qtDrawerWidget.h"
#include "qtExpander.h"

QTE_IMPLEMENT_D_FUNC(qtDrawer)

//-----------------------------------------------------------------------------
class qtDrawerPrivate
{
public:
  qtDrawerPrivate(qtDrawer*);

  void setExpander(bool shouldExist, bool shouldEnable);
  void setIndent(int);

protected:
  QTE_DECLARE_PUBLIC_PTR(qtDrawer)

  qtExpander* expander;
  bool expanded;
  qtDrawer::ExpanderPolicy expanderPolicy;

  qtDrawerWidget* container;
  QGridLayout* containerLayout;

  qtDrawer* parent;
  QList<qtDrawer*> children;
  QMap<int, QWidget*> contentWidgets;
  QHBoxLayout* indentLayout;

  int row;
  int indent;

  bool visibilityFlag;

private:
  QTE_DECLARE_PUBLIC(qtDrawer)
};

//-----------------------------------------------------------------------------
qtDrawerPrivate::qtDrawerPrivate(qtDrawer* q)
  : q_ptr(q), expander(0), expanded(false),
    expanderPolicy(qtDrawer::ExpanderAsNeeded),
    container(0), containerLayout(0), parent(0),
    indentLayout(0), row(-1), indent(-1),
    visibilityFlag(true)
{
}

//-----------------------------------------------------------------------------
void qtDrawerPrivate::setExpander(bool shouldExist, bool shouldEnable)
{
  if (!!this->expander != shouldExist)
    {
    if (this->expander)
      {
      // Remove expander
      delete this->expander;
      this->expander = 0;
      }
    else
      {
      // Create expander
      QTE_Q(qtDrawer);
      this->expander = new qtExpander(false, q);
      this->expander->setChecked(this->expanded);
      q->connect(this->expander, SIGNAL(toggled(bool)),
                 q, SLOT(setExpanded(bool)));
      }

    // Reindent to add/remove expander in indent layout
    this->setIndent(this->indent);
    }

  if (this->expander)
    {
    this->expander->setEnabled(shouldEnable);
    }
}

//-----------------------------------------------------------------------------
void qtDrawerPrivate::setIndent(int newIndent)
{
  QTE_Q(qtDrawer);

  this->indent = newIndent;

  if (this->indentLayout)
    {
    // Temporarily remove expander and main content widget
    if (this->expander)
      {
      this->indentLayout->removeWidget(this->expander);
      }
    if (this->contentWidgets.contains(0))
      {
      this->indentLayout->removeWidget(this->contentWidgets.value(0));
      }
    // Remove existing spacing
    delete this->indentLayout->takeAt(0);
    }
  else
    {
    this->indentLayout = new QHBoxLayout;
    this->indentLayout->setContentsMargins(0, 0, 0, 0);
    q->setLayout(this->indentLayout);
    }

  // Add spacing for indentation
  if (newIndent > 0)
    {
    int s = this->indentLayout->spacing();
    // If layout spacing is unset, try to get the spacing value from the style
    if (s < 0)
      {
      QStyle* style = QApplication::style();
      s = style->layoutSpacing(QSizePolicy::ToolButton,
                               QSizePolicy::DefaultType,
                               Qt::Horizontal);
      if (s < 0)
        {
        s = style->pixelMetric(QStyle::PM_DefaultLayoutSpacing);
        }
      }
    int iw = (qtExpander::size(q->style()).width() + s);
    this->indentLayout->addSpacing(iw * indent);
    }

  // (Re)add expander and main content widget
  if (this->expander)
    {
    this->indentLayout->addWidget(expander);
    }
  if (this->contentWidgets.contains(0))
    {
    this->indentLayout->addWidget(this->contentWidgets.value(0));
    }
}

//-----------------------------------------------------------------------------
qtDrawer::qtDrawer(qtDrawerWidget* container, QGridLayout* layout)
  : QWidget(container), d_ptr(new qtDrawerPrivate(this))
{
  QTE_D(qtDrawer);
  d->container = container;
  d->containerLayout = layout;
}

//-----------------------------------------------------------------------------
qtDrawer::qtDrawer(qtDrawer* parent, qtDrawer* nextSibling)
  : QWidget(parent), d_ptr(new qtDrawerPrivate(this))
{
  QTE_D(qtDrawer);
  qtDrawerPrivate* p = parent->d_func();

  d->container = p->container;
  d->containerLayout = p->containerLayout;
  d->parent = parent;

  d->setIndent(p->indent + 1);

  parent->addChild(this, nextSibling);

  parent->setChildVisibility(this);
}

//-----------------------------------------------------------------------------
qtDrawer::~qtDrawer()
{
  QTE_D(qtDrawer);

  // Remove children
  while (d->children.count())
    {
    qtDrawer* child = d->children.last();
    this->removeChild(child);
    delete child;
    }

  // Remove content widgets
  d->containerLayout->removeWidget(this);
  foreach (auto const col, d->contentWidgets.keys())
    {
    if (col != 0)
      {
      QWidget* widget = d->contentWidgets.value(col);
      d->containerLayout->removeWidget(widget);
      delete widget;
      }
    }

  // Remove ourself from our parent
  if (d->parent)
    {
    d->parent->removeChild(this);
    }
}

//-----------------------------------------------------------------------------
void qtDrawer::addWidget(QWidget* widget, int col)
{
  QTE_D(qtDrawer);

  if (col == 0)
    {
    if (d->contentWidgets.contains(0))
      {
      d->indentLayout->removeWidget(d->contentWidgets.value(0));
      }
    d->indentLayout->addWidget(widget);
    }
  else
    {
    if (d->contentWidgets.contains(col))
      {
      d->containerLayout->removeWidget(d->contentWidgets.value(col));
      }
    d->containerLayout->addWidget(widget, d->row, col);
    }

  d->contentWidgets.insert(col, widget);
  widget->setVisible(this->isVisibleTo(d->container));
}

//-----------------------------------------------------------------------------
qtDrawer::ExpanderPolicy qtDrawer::expanderPolicy() const
{
  QTE_D_CONST(qtDrawer);
  return d->expanderPolicy;
}

//-----------------------------------------------------------------------------
void qtDrawer::setExpanderPolicy(qtDrawer::ExpanderPolicy newPolicy)
{
  QTE_D(qtDrawer);

  if (d->expanderPolicy == newPolicy)
    {
    return;
    }

  // Set new policy
  d->expanderPolicy = newPolicy;

  // Set expander visibility based on policy and need
  if (d->parent)
    {
    bool needExpander = d->children.count();
    bool wantExpander = (newPolicy == ExpanderAlwaysShown);
    d->setExpander(wantExpander || needExpander, needExpander);
    }
}

//-----------------------------------------------------------------------------
void qtDrawer::addChild(qtDrawer* child, qtDrawer* before)
{
  QTE_D(qtDrawer);

  // Create expander if not already created
  if (d->parent)
    {
    d->setExpander(true, true);
    }

  // Determine insertion point
  int row;
  int index = (before ? d->children.indexOf(before) : -1);
  if (index >= 0)
    {
    // Add at 'before's row, shifting children down
    row = before->d_func()->row;
    this->shiftChildrenDown(before);
    before->shiftDown();
    }
  else
    {
    // 'before' is 0 or not a sibling; add to bottom and shift later siblings
    if (d->parent)
      {
      d->parent->shiftChildrenDown(this);
      }
    row = d->row + this->countDescendants() + 1;
    index = d->children.count();
    }

  // Set child's row
  child->d_func()->row = row;

  // Add new drawer to layout
  d->containerLayout->addWidget(child, row, 0);

  // Add new drawer to children
  d->children.insert(index, child);
}

//-----------------------------------------------------------------------------
void qtDrawer::removeChild(qtDrawer* child)
{
  QTE_D(qtDrawer);

  // Find child's index
  int index = d->children.indexOf(child);
  if (index < 0)
    {
    return;
    }

  // Shift later siblings and remove child
  this->shiftChildrenUp(child);
  d->children.removeAt(index);

  // Remove expander if no longer needed
  if (d->parent && d->children.isEmpty())
    {
    d->setExpander(d->expanderPolicy == ExpanderAlwaysShown, false);
    }
}

//-----------------------------------------------------------------------------
int qtDrawer::countDescendants() const
{
  QTE_D_CONST(qtDrawer);

  int count = d->children.count();
  foreach (auto const* const child, d->children)
    count += child->countDescendants();

  return count;
}

//-----------------------------------------------------------------------------
void qtDrawer::shiftChildrenDown(qtDrawer* afterChild)
{
  QTE_D(qtDrawer);

  // Disable updates while we rearrange things
  bool enableUpdates = d->container->updatesEnabled();
  d->container->setUpdatesEnabled(false);

  // Shift later siblings, but only when shifting a partial set; shiftDown()
  // shifts everyone, and we don't want to walk up in that case or things will
  // get shifted twice
  if (afterChild && d->parent)
    {
    d->parent->shiftChildrenDown(this);
    }

  // Shift down own children
  int n = d->children.count();
  while (n-- && d->children[n] != afterChild)
    {
    d->children[n]->shiftDown();
    }

  // Restore updates state
  d->container->setUpdatesEnabled(enableUpdates);
}

//-----------------------------------------------------------------------------
void qtDrawer::shiftChildrenUp(qtDrawer* afterChild)
{
  QTE_D(qtDrawer);

  // Disable updates while we rearrange things
  bool enableUpdates = d->container->updatesEnabled();
  d->container->setUpdatesEnabled(false);

  // Shift down own children
  int n = (afterChild ? d->children.indexOf(afterChild) : -1);
  while (++n < d->children.count())
    {
    d->children[n]->shiftUp();
    }

  // Shift later siblings, but only when shifting a partial set; shiftUp()
  // shifts everyone, and we don't want to walk up in that case or things will
  // get shifted twice
  if (afterChild && d->parent)
    {
    d->parent->shiftChildrenUp(this);
    }

  // Restore updates state
  d->container->setUpdatesEnabled(enableUpdates);
}

//-----------------------------------------------------------------------------
void qtDrawer::shiftDown()
{
  QTE_D(qtDrawer);

  d->row++;
  this->shiftChildrenDown();
  this->shift();
}

//-----------------------------------------------------------------------------
void qtDrawer::shiftUp()
{
  QTE_D(qtDrawer);

  d->row++;
  this->shiftChildrenUp();
  this->shift();
}

//-----------------------------------------------------------------------------
void qtDrawer::shift()
{
  QTE_D(qtDrawer);

  // Shift self
  d->containerLayout->removeWidget(this);
  d->containerLayout->addWidget(this, d->row, 0);

  // Shift other content widgets
  foreach (auto const col, d->contentWidgets.keys())
    {
    if (col != 0)
      {
      QWidget* widget = d->contentWidgets.value(col);
      d->containerLayout->removeWidget(widget);
      d->containerLayout->addWidget(widget, d->row, col);
      }
    }
}

//-----------------------------------------------------------------------------
void qtDrawer::clear()
{
  QTE_D(qtDrawer);

  foreach (auto const child, d->children)
    {
    child->clear();
    delete child;
    }
  d->children.clear();

  qDeleteAll(d->contentWidgets);
  d->contentWidgets.clear();
}

//-----------------------------------------------------------------------------
qtDrawer* qtDrawer::parent()
{
  QTE_D(qtDrawer);
  return d->parent;
}

//-----------------------------------------------------------------------------
void qtDrawer::setExpanded(bool expanded)
{
  QTE_D(qtDrawer);

  if (!d->expander)
    {
    d->expanded = expanded;
    return;
    }

  if (d->expanded != expanded)
    {
    d->expanded = expanded;
    d->expander->setChecked(expanded);
    this->setChildVisibility();
    emit this->expandToggled(expanded);
    }
}

//-----------------------------------------------------------------------------
void qtDrawer::setVisibility(bool visibility)
{
  QTE_D(qtDrawer);

  if (d->visibilityFlag == visibility)
    {
    return;
    }

  d->visibilityFlag = visibility;

  if (!visibility)
    {
    this->setVisible(false);
    }
  else
    {
    if (d->parent)
      {
      d->parent->setChildVisibility(this);
      }
    else
      {
      this->setVisible(true);
      }
    }
}

//-----------------------------------------------------------------------------
void qtDrawer::setVisible(bool visibility)
{
  QTE_D(qtDrawer);

  visibility = visibility && d->visibilityFlag;

  this->setUpdatesEnabled(false);
  QWidget::setVisible(visibility);
  foreach (auto const widget, d->contentWidgets)
    widget->setVisible(visibility);

  this->setChildVisibility();

  this->setUpdatesEnabled(true);
}

//-----------------------------------------------------------------------------
void qtDrawer::setChildVisibility(qtDrawer* child)
{
  QTE_D(qtDrawer);

  bool visibility = this->isVisibleTo(d->container);
  if (d->expander)
    {
    visibility = visibility && d->expander->isChecked();
    }

  if (child)
    {
    child->setVisible(visibility);
    }
  else
    {
    foreach (auto const child, d->children)
      child->setVisible(visibility);
    }
}
