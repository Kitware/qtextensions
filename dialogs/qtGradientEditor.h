/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
    qtGradientEditor(QWidget* parent = 0, Qt::WindowFlags flags = 0);
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
