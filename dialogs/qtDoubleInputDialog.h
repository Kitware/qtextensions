/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDoubleInputDialog_h
#define __qtDoubleInputDialog_h

#include <QDialog>
#include <QPair>

#include "../core/qtGlobal.h"

class QDoubleSpinBox;
class qtDoubleInputDialogPrivate;

class QTE_EXPORT qtDoubleInputDialog : protected QDialog
{
  Q_OBJECT

public:
  static double getDouble(
    QWidget* parent, const QString& title, const QString& label, double value,
    double minValue, double maxValue, bool* okay = 0,
    Qt::WindowFlags flags = 0);

  static double getDouble(
    QWidget* parent, const QString& title, const QString& label, double value,
    double minValue, double maxValue, int decimals = 1, double step = 1.0,
    bool* okay = 0, Qt::WindowFlags flags = 0);

  static double getDouble(
    QWidget* parent, const QString& title, const QString& label, double value,
    double minValue, double maxValue, const QString& specialValueText,
    int decimals = 1, double step = 1.0, bool* okay = 0,
    Qt::WindowFlags flags = 0);

protected:
  QTE_DECLARE_PRIVATE_PTR(qtDoubleInputDialog)

  qtDoubleInputDialog(QWidget* parent, Qt::WindowFlags flags);
  virtual ~qtDoubleInputDialog();

  void setLabel(const QString&);

  void setSpinProperties(double value, double minValue, double maxValue,
                         int decimals, double step);

  void setSpecialValueText(const QString&);
  double value();

private:
  QTE_DECLARE_PRIVATE(qtDoubleInputDialog)
  Q_DISABLE_COPY(qtDoubleInputDialog)
};

#endif
