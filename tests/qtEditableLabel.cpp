// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtEditableLabel.h"

#include <QBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>

QTE_IMPLEMENT_D_FUNC(qtEditableLabel)

//-----------------------------------------------------------------------------
class qtEditableLabelPrivate : public QLineEdit
{
public:
  explicit qtEditableLabelPrivate(qtEditableLabel*);

protected:
  QTE_DECLARE_PUBLIC_PTR(qtEditableLabel)

  virtual void keyPressEvent(QKeyEvent*);
};

//-----------------------------------------------------------------------------
qtEditableLabelPrivate::qtEditableLabelPrivate(qtEditableLabel* q)
  : QLineEdit(q), q_ptr(q)
{
  this->setFrame(false);
  this->setVisible(false);

  connect(this, SIGNAL(editingFinished()), q, SLOT(endEditing()));
}

//-----------------------------------------------------------------------------
qtEditableLabel::qtEditableLabel(QWidget* parent)
  : QLabel(parent), d_ptr(new qtEditableLabelPrivate(this))
{
}

//-----------------------------------------------------------------------------
void qtEditableLabelPrivate::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape)
    q_ptr->cancelEditing();
  else
    QLineEdit::keyPressEvent(event);
}

//-----------------------------------------------------------------------------
qtEditableLabel::~qtEditableLabel()
{
}

//-----------------------------------------------------------------------------
void qtEditableLabel::beginEditing()
{
  QTE_D(qtEditableLabel);
  d->setText(this->text());
  d->selectAll();
  d->show();
  d->setFocus();
  d->setGeometry(0, 0, this->width(), this->height());
}

//-----------------------------------------------------------------------------
void qtEditableLabel::endEditing()
{
  this->setText(this->d_ptr->text());
  this->cancelEditing();
}

//-----------------------------------------------------------------------------
void qtEditableLabel::cancelEditing()
{
  QTE_D(qtEditableLabel);
  d->blockSignals(true);
  d->hide();
  d->blockSignals(false);
}

//-----------------------------------------------------------------------------
void qtEditableLabel::mouseDoubleClickEvent(QMouseEvent* event)
{
  this->beginEditing();
  QWidget::mouseDoubleClickEvent(event);
}
