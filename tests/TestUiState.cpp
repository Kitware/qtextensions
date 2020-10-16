// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#define TEST_OBJECT_NAME t_obj

#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QScopedPointer>
#include <QSettings>
#include <QSpinBox>
#include <QTemporaryFile>

#include <ctime>

#include "../core/qtTest.h"

#include "../util/qtRand.h"
#include "../util/qtUiState.h"

#define MAKE_STATE(_tempFile, _state) \
  QTemporaryFile _tempFile("settings-XXXXXX.ini"); \
  if (TEST_EQUAL(_tempFile.open(), true)) return 1; \
  qtUiState _state(new QSettings(_tempFile.fileName(), QSettings::IniFormat))

//-----------------------------------------------------------------------------
class IntItem : public qtUiState::AbstractItem
{
public:
  IntItem(int& value) :
    Value(value) {}

  virtual QVariant value() const
    { return this->Value; }

  virtual void setValue(const QVariant& data)
    { this->Value = data.toInt(); }

protected:
  int& Value;
};

//-----------------------------------------------------------------------------
int testCustomItem(qtTest& t_obj)
{
  MAKE_STATE(tf, s);

  int value = 2;
  s.map("value", new IntItem(value));

  s.restore();
  TEST_EQUAL(value, 2);

  value = 3;
  s.restore();
  TEST_EQUAL(value, 3);

  value = 4;
  s.save();
  TEST_EQUAL(value, 4);

  value = 2;
  s.restore();
  TEST_EQUAL(value, 4);

  return 0;
}

//-----------------------------------------------------------------------------
int randomize(int other, int max)
{
  int data = qtRand(max);
  return (data == other ? randomize(other, max) : data);
}

//-----------------------------------------------------------------------------
QString randomize(const QString& other)
{
  static const char16_t chars[] =
    u"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    u"0123456789,./;'[]\\`-=<>?:\"{}|~!@#$%^&*()_+";

  QString data;
  for (int i = qtRand(64); i; --i)
    {
    data += QChar{chars[qtRand(sizeof(chars) / sizeof(*chars))]};
    }
  return (data == other ? randomize(other) : data);
}

//-----------------------------------------------------------------------------
int testBuiltin(qtTest& t_obj)
{
  MAKE_STATE(tf, s);

  // Create widgets
  QScopedPointer<QObject> object(new QObject);
  QScopedPointer<QAction> action(new QAction(object.data()));
  QScopedPointer<QCheckBox> checkBox(new QCheckBox);
  QScopedPointer<QGroupBox> groupBox(new QGroupBox);
  QScopedPointer<QLineEdit> lineEdit(new QLineEdit);
  QScopedPointer<QSpinBox> spinBox(new QSpinBox);
  QScopedPointer<QDoubleSpinBox> doubleSpinBox(new QDoubleSpinBox);
  action->setCheckable(true);
  groupBox->setCheckable(true);
  doubleSpinBox->setDecimals(2);

  const int maxDoubleInt =
    static_cast<int>(floor(doubleSpinBox->maximum() * 1e2));

  // Create random data
  const bool actionChecked = (qtRandD() < 0.5);
  const bool checkBoxChecked = (qtRandD() < 0.5);
  const bool groupBoxChecked = (qtRandD() < 0.5);
  const QString lineEditText = randomize(QString());
  const int spinBoxValue = qtRand(spinBox->maximum());
  const int doubleSpinBoxValue = qtRand(maxDoubleInt);

  // Set initial data on widgets
  action->setChecked(actionChecked);
  checkBox->setChecked(checkBoxChecked);
  groupBox->setChecked(groupBoxChecked);
  lineEdit->setText(lineEditText);
  spinBox->setValue(spinBoxValue);
  doubleSpinBox->setValue(doubleSpinBoxValue * 1e-2);

  // Define map and save states
  s.mapChecked("action", action.data());
  s.mapChecked("checkBox", checkBox.data());
  s.mapChecked("groupBox", groupBox.data());
  s.mapText("lineEdit", lineEdit.data());
  s.mapValue("spinBox", spinBox.data());
  s.mapValue("doubleSpinBox", doubleSpinBox.data());
  s.save();

  // Set different values
  action->setChecked(!actionChecked);
  checkBox->setChecked(!checkBoxChecked);
  groupBox->setChecked(!groupBoxChecked);
  lineEdit->setText(randomize(lineEditText));
  spinBox->setValue(randomize(spinBoxValue, spinBox->maximum()));
  doubleSpinBox->setValue(randomize(doubleSpinBoxValue, maxDoubleInt) * 1e-2);

  // Restore states
  s.restore();

  // Test restored values
  TEST_EQUAL(action->isChecked(), actionChecked);
  TEST_EQUAL(checkBox->isChecked(), checkBoxChecked);
  TEST_EQUAL(groupBox->isChecked(), groupBoxChecked);
  TEST_EQUAL(lineEdit->text(), lineEditText);
  TEST_EQUAL(spinBox->value(), spinBoxValue);
  TEST_EQUAL(doubleSpinBox->value(), doubleSpinBoxValue * 1e-2);

  return 0;
}

//-----------------------------------------------------------------------------
int testCurrentGroup(qtTest& t_obj)
{
  MAKE_STATE(tf, s);

  int dog = 2, cat = 3, cow = 4;
  QString house("house"), barn("barn/stall");

  TEST_EQUAL(s.currentGroup(), QString(""));
  s.map("dog", new IntItem(dog));

  s.setCurrentGroup(house);
  TEST_EQUAL(s.currentGroup(), house);
  s.map("cat", new IntItem(cat));

  s.setCurrentGroup(barn);
  TEST_EQUAL(s.currentGroup(), barn);
  s.map("cow", new IntItem(cow));

  s.save();

  QSettings rawSettings(tf.fileName(), QSettings::IniFormat);

  TEST_EQUAL(rawSettings.value("dog", 0).toInt(), 2);
  TEST_EQUAL(rawSettings.value(house + "/cat", 0).toInt(), 3);
  TEST_EQUAL(rawSettings.value(barn + "/cow", 0).toInt(), 4);

  return 0;
}

//-----------------------------------------------------------------------------
int testExpected(qtTest& t_obj, int actual, int expected, const QString& name)
{
  // Check parsed value
  if (actual != expected)
    {
    t_obj.out() << t_obj.where() << qPrintable(name) << " has value "
                << actual << " (expected " << expected << ")\n";
    return t_obj.setTestResult(1);
    }
  return 0;
}

//-----------------------------------------------------------------------------
int testNamed(
  qtTest& t_obj, int a, int b, int c, int d, int xa, int xb, int xc, int xd)
{
  TEST_CALL(testExpected, a, xa, "'a'");
  TEST_CALL(testExpected, b, xb, "'b'");
  TEST_CALL(testExpected, c, xc, "'c'");
  TEST_CALL(testExpected, d, xd, "'d'");
  return 0;
}

//-----------------------------------------------------------------------------
#define NAMED_RESET() do { \
  a = 1; b = 2; c = 3; d = 4; \
  s.save(); \
  a = b = c = d = 0; \
  } while(0)

#define TEST_NAMED(_xa, _xb, _xc, _xd) \
  TEST_CALL(testNamed, a, b, c, d, _xa, _xb, _xc, _xd)

//-----------------------------------------------------------------------------
int testNamedSave(qtTest& t_obj)
{
  MAKE_STATE(tf, s);

  int a, b, c, d;

  s.map("fubar", new IntItem(a));
  s.map("metal/bar", new IntItem(b));
  s.map("bar/none", new IntItem(c));
  s.map("full/metal/jacket", new IntItem(d));

  // Test non-matching key
  NAMED_RESET();
  s.save("a");
  s.restore();
  TEST_NAMED(1,2,3,4);

  // Test matching-anywhere key
  NAMED_RESET();
  s.save("bar");
  s.restore();
  TEST_NAMED(1,0,0,4);

  // Test matches-prefix keys
  NAMED_RESET();
  s.save("/bar");
  s.restore();
  TEST_NAMED(1,2,0,4);
  NAMED_RESET();
  s.save("/fubar");
  s.restore();
  TEST_NAMED(0,2,3,4);

  // Test matches-group key
  NAMED_RESET();
  s.save("metal/");
  s.restore();
  TEST_NAMED(1,0,3,0);

  // Test matches-initial-group key
  NAMED_RESET();
  s.save("/metal/");
  s.restore();
  TEST_NAMED(1,0,3,4);

  return 0;
}

//-----------------------------------------------------------------------------
int testNamedRestore(qtTest& t_obj)
{
  MAKE_STATE(tf, s);

  int a, b, c, d;

  s.map("fubar", new IntItem(a));
  s.map("metal/bar", new IntItem(b));
  s.map("bar/none", new IntItem(c));
  s.map("full/metal/jacket", new IntItem(d));

  // Test non-matching key
  NAMED_RESET();
  s.restore("a");
  TEST_NAMED(0,0,0,0);

  // Test matching-anywhere key
  NAMED_RESET();
  s.restore("bar");
  TEST_NAMED(0,2,3,0);

  // Test matches-prefix keys
  NAMED_RESET();
  s.restore("/bar");
  TEST_NAMED(0,0,3,0);
  NAMED_RESET();
  s.restore("/fubar");
  TEST_NAMED(1,0,0,0);

  // Test matches-group key
  NAMED_RESET();
  s.restore("metal/");
  TEST_NAMED(0,2,0,4);

  // Test matches-initial-group key
  NAMED_RESET();
  s.restore("/metal/");
  TEST_NAMED(0,2,0,0);

  return 0;
}

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  QApplication app(argc, argv); // Needed to construct widgets
  qtTest t_obj;

  qsrand(static_cast<uint>(time(0)));

  t_obj.runSuite("Custom Mapping Test", testCustomItem);
  t_obj.runSuite("Built-in Mapping Tests", testBuiltin);
  t_obj.runSuite("Current Group Tests", testCurrentGroup);
  t_obj.runSuite("Save Named Tests", testNamedSave);
  t_obj.runSuite("Restore Named Tests", testNamedRestore);
  return t_obj.result();
}
