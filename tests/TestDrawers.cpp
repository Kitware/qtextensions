// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "TestDrawers.h"

#include <QApplication>
#include <QGridLayout>
#include <QScrollArea>
#include <QToolButton>
#include <QVBoxLayout>

#include "../widgets/qtDrawer.h"
#include "../widgets/qtDrawerWidget.h"

#include "qtEditableLabel.h"

//-----------------------------------------------------------------------------
QString makeText(void* p)
{
  quintptr n = reinterpret_cast<quintptr>(p);
  return QString("drawer 0x%1").arg(n, 0, 16);
}

//-----------------------------------------------------------------------------
TestDrawersWidget::TestDrawersWidget(QWidget* parent)
  : QWidget(parent), drawerWidget(new qtDrawerWidget)
{
  // Set up UI
  QGridLayout* lo = new QGridLayout;
  this->setLayout(lo);

  QScrollArea* sa = new QScrollArea;
  QWidget* sac = new QWidget;
  sa->setFrameShape(QFrame::NoFrame);
  sa->setWidget(sac);
  sa->setWidgetResizable(true);

  QVBoxLayout* li = new QVBoxLayout;
  sac->setLayout(li);
  li->addWidget(this->drawerWidget);
  li->addStretch();
  lo->addWidget(sa, 0, 0, 1, 3);

  QToolButton* bar = new QToolButton;
  bar->setIcon(QIcon(":icons/add"));
  bar->setText("Add new drawer to root");
  bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  lo->addWidget(bar, 1, 0);
  connect(bar, SIGNAL(clicked(bool)), this, SLOT(addRootDrawer()));

  QToolButton* bc = new QToolButton;
  bc->setIcon(QIcon(":icons/clear"));
  bc->setText("Remove all drawers");
  bc->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  lo->addWidget(bc, 1, 1);
  connect(bc, SIGNAL(clicked(bool)), this, SLOT(clearDrawers()));

  lo->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding), 1, 2);

  connect(&this->mapAddBefore, SIGNAL(mapped(QWidget*)),
          this, SLOT(addDrawerBefore(QWidget*)));
  connect(&this->mapAddChild, SIGNAL(mapped(QWidget*)),
          this, SLOT(addDrawerChild(QWidget*)));
  connect(&this->mapRemove, SIGNAL(mapped(QWidget*)),
          this, SLOT(removeDrawer(QWidget*)));

  this->addRootDrawer();
}

//-----------------------------------------------------------------------------
TestDrawersWidget::~TestDrawersWidget()
{
}

//-----------------------------------------------------------------------------
void TestDrawersWidget::addRootDrawer()
{
  qtDrawer* d = this->drawerWidget->addDrawer();
  this->setupDrawer(d);
}

//-----------------------------------------------------------------------------
void TestDrawersWidget::clearDrawers()
{
  this->drawerWidget->clear();
}

//-----------------------------------------------------------------------------
void TestDrawersWidget::addDrawerBefore(QWidget* object)
{
  qtDrawer* d = qobject_cast<qtDrawer*>(object);
  if (d)
    {
    qtDrawer* s = d;
    d = new qtDrawer(s->parent(), s);
    this->setupDrawer(d);
    }
}

//-----------------------------------------------------------------------------
void TestDrawersWidget::addDrawerChild(QWidget* object)
{
  qtDrawer* d = qobject_cast<qtDrawer*>(object);
  if (d)
    {
    qtDrawer* p = d;
    d = new qtDrawer(p);
    this->setupDrawer(d);
    p->setExpanded(true);
    }
}

//-----------------------------------------------------------------------------
void TestDrawersWidget::removeDrawer(QWidget* object)
{
  qtDrawer* d = qobject_cast<qtDrawer*>(object);
  d && (d->deleteLater(), false);
}

//-----------------------------------------------------------------------------
void TestDrawersWidget::setupDrawer(qtDrawer* d)
{
  // Add label
  qtEditableLabel* label = new qtEditableLabel;
  label->setText(makeText(d));
  d->addWidget(label);

  // Set up 'add before' button
  QToolButton* bab = new QToolButton;
  bab->setIcon(QIcon(":icons/add-before"));
  d->addWidget(bab, 1);
  this->mapAddBefore.setMapping(bab, d);
  connect(bab, SIGNAL(clicked(bool)), &this->mapAddBefore, SLOT(map()));

  // Set up 'add child' button
  QToolButton* bac = new QToolButton;
  bac->setIcon(QIcon(":icons/add-child"));
  d->addWidget(bac, 2);
  this->mapAddChild.setMapping(bac, d);
  connect(bac, SIGNAL(clicked(bool)), &this->mapAddChild, SLOT(map()));

  // Set up 'remove' button
  QToolButton* br = new QToolButton;
  br->setIcon(QIcon(":icons/remove"));
  d->addWidget(br, 3);
  this->mapRemove.setMapping(br, d);
  connect(br, SIGNAL(clicked(bool)), &this->mapRemove, SLOT(map()));
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  TestDrawersWidget window;
  window.show();
  return app.exec();
}
