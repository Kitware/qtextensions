// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDrawer_h
#define __qtDrawer_h

#include <QWidget>

#include "../core/qtGlobal.h"

class QGridLayout;

class qtDrawerWidget;
class qtDrawerPrivate;

class QTE_EXPORT qtDrawer : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(ExpanderPolicy expanderPolicy READ expanderPolicy
                                           WRITE setExpanderPolicy)

public:
  enum ExpanderPolicy
    {
    ExpanderAsNeeded = 0,
    ExpanderAlwaysShown = 1
    };

  explicit qtDrawer(qtDrawer* parent, qtDrawer* nextSibling = nullptr);
  virtual ~qtDrawer();

  qtDrawer* parent();

  virtual void addWidget(QWidget* widget, int col = 0);

  ExpanderPolicy expanderPolicy() const;
  void setExpanderPolicy(ExpanderPolicy);

signals:
  void expandToggled(bool);

public slots:
  void setExpanded(bool);
  virtual void setVisible(bool);
  virtual void setVisibility(bool);

protected:
  QTE_DECLARE_PRIVATE_RPTR(qtDrawer)
  friend class qtDrawerWidget;

  qtDrawer(qtDrawerWidget* container, QGridLayout* layout);

  virtual void addChild(qtDrawer* child, qtDrawer* before = nullptr);
  virtual void removeChild(qtDrawer* child);
  void setChildVisibility(qtDrawer* child = nullptr);

  int countDescendants() const;
  void shiftChildrenDown(qtDrawer* afterChild = nullptr);
  void shiftChildrenUp(qtDrawer* afterChild = nullptr);
  void shiftDown();
  void shiftUp();
  void shift();

  virtual void clear();

private:
  QTE_DECLARE_PRIVATE(qtDrawer)
  QTE_DISABLE_COPY(qtDrawer)
};

#endif
