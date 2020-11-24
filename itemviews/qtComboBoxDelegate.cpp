// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtComboBoxDelegate.h"

#include "../core/qtIndexRange.h"

#include <QApplication>
#include <QComboBox>
#include <QTimer>

//-----------------------------------------------------------------------------
class qtDelegateComboBox : public QComboBox
{
public:
    qtDelegateComboBox(QWidget* parent) : QComboBox{parent}
    {
        this->blockHidePopupTimer.setSingleShot(true);
    }

    void showPopup() override
    {
        this->blockHidePopupTimer.start(QApplication::doubleClickInterval());
        QComboBox::showPopup();

    }

    void hidePopup() override
    {
        if (!this->blockHidePopupTimer.isActive())
            QComboBox::hidePopup();
    }

private:
    QTimer blockHidePopupTimer;
};

//-----------------------------------------------------------------------------
qtComboBoxDelegate::qtComboBoxDelegate(QObject* parent)
    : qtAbstractListDelegate(parent)
{
}

//-----------------------------------------------------------------------------
qtComboBoxDelegate::~qtComboBoxDelegate()
{
}

//-----------------------------------------------------------------------------
QWidget* qtComboBoxDelegate::createListEditor(QWidget* parent) const
{
    // Create combo box
    auto* const box = new qtDelegateComboBox{parent};
    box->setFocusPolicy(Qt::StrongFocus);
    connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &qtComboBoxDelegate::editorValueChanged);

    // Fill combo box
    for (auto const& name : this->valueNames())
        box->addItem(name, this->valueData(name));

    // Queue request for box to show pop-up (needs to be delayed because we
    // don't know the geometry yet)
    QMetaObject::invokeMethod(box, &qtDelegateComboBox::showPopup,
                              Qt::QueuedConnection);

    // Done
    return box;
}

//-----------------------------------------------------------------------------
void qtComboBoxDelegate::setListEditorData(
    QWidget* editor, QVariant const& newData) const
{
    auto* const box = qobject_cast<QComboBox*>(editor);

    auto i = decltype(box->count()){-1};
    for (auto const j : qtIndexRange(box->count()))
    {
        if (this->compareData(newData, box->itemData(j)))
        {
            i = j;
            break;
        }
    }
    box->setCurrentIndex(i);
}

//-----------------------------------------------------------------------------
void qtComboBoxDelegate::setModelData(
    QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    auto* const box = qobject_cast<QComboBox*>(editor);
    auto const& newData = box->itemData(box->currentIndex());
    this->setModelData(editor, box->currentText(), newData, model, index);
}

//-----------------------------------------------------------------------------
void qtComboBoxDelegate::updateEditorGeometry(
    QWidget* editor,
    QStyleOptionViewItem const& option,
    QModelIndex const&) const
{
    auto* const box = qobject_cast<QComboBox*>(editor);
    box->setGeometry(option.rect);
}
