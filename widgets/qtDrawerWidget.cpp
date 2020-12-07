// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include <QGridLayout>

#include "qtDrawer.h"
#include "qtDrawerWidget.h"

QTE_IMPLEMENT_D_FUNC(qtDrawerWidget)

//-----------------------------------------------------------------------------
class qtDrawerWidgetPrivate
{
public:
  qtDrawerWidgetPrivate() : root{nullptr} {}

  qtDrawer* root;
};

//-----------------------------------------------------------------------------
qtDrawerWidget::qtDrawerWidget(QWidget* parent)
  : QWidget(parent), d_ptr(new qtDrawerWidgetPrivate)
{
  this->setLayout(new QGridLayout);
}

//-----------------------------------------------------------------------------
qtDrawerWidget::~qtDrawerWidget()
{
  // Explicitly remove drawers... if we rely on QObject to reap its children,
  // our layout can be deleted before the drawers, which need the layout to
  // clean themselves up
  qtDrawer* r = this->root();
  r->clear();
  delete r;
}

//-----------------------------------------------------------------------------
qtDrawer* qtDrawerWidget::addDrawer(qtDrawer* nextSibling)
{
  return new qtDrawer(this->root(), nextSibling);
}

//-----------------------------------------------------------------------------
void qtDrawerWidget::clear()
{
  this->root()->clear();
}

//-----------------------------------------------------------------------------
qtDrawer* qtDrawerWidget::root()
{
  QTE_D(qtDrawerWidget);

  if (!d->root)
    {
    d->root = this->createRoot();
    }

  return d->root;
}

//-----------------------------------------------------------------------------
qtDrawer* qtDrawerWidget::createRoot()
{
  QGridLayout* layout = qobject_cast<QGridLayout*>(this->layout());
  return new qtDrawer(this, layout);
}
