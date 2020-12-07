// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtGradientEditor_h
#define __qtGradientEditor_h

#include "../core/qtGlobal.h"

#include <QDialog>

class qtGradient;

class qtGradientEditorPrivate;

class QTE_EXPORT qtGradientEditor : public QDialog
{
    Q_OBJECT

public:
    qtGradientEditor(QWidget* parent = 0, Qt::WindowFlags flags = {});
    virtual ~qtGradientEditor();

    qtGradient gradient() const;

public slots:
    void setGradient(qtGradient const&);

protected slots:
    void addStop();
    void removeStops();

    void updateStops();
    void updateInterpolationMode();

protected:
    QTE_DECLARE_PRIVATE_RPTR(qtGradientEditor)

private:
    QTE_DECLARE_PRIVATE(qtGradientEditor)
    QTE_DISABLE_COPY(qtGradientEditor)
};

#endif
