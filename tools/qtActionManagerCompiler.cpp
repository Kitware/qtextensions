/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "qteVersion.h"

#include "../core/qtEnumerate.h"
#include "../core/qtGlobal.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QFileInfo>
#include <QMultiHash>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <cstdio>

typedef QHash<QString, QString> ObjectProperties;
typedef QHash<QString, ObjectProperties> ActionMap;
typedef QMultiHash<QString, QString> ActionGroupMap;

//-----------------------------------------------------------------------------
QDebug out(QtMsgType type = QtDebugMsg, QString file = QString(),
           uint line = 0, uint col = 0)
{
  QString prefix = "qte-amc:";
  if (!file.isEmpty())
    {
    prefix += ' ' + file + ':';
    if (line > 0)
      {
      prefix += QString::number(line) + ':';
      if (col > 0)
        {
        prefix += QString::number(col) + ':';
        }
      }
    }

  if (type == QtDebugMsg)
    {
    return QDebug(type);
    }
  return QDebug(type) << qPrintable(prefix);
}

//-----------------------------------------------------------------------------
QDebug err(QString file = QString(), uint line = 0, uint col = 0)
{
  return out(QtCriticalMsg, file, line, col) << "error:";
}

//-----------------------------------------------------------------------------
QDebug warn(QString file = QString(), uint line = 0, uint col = 0)
{
  return out(QtWarningMsg, file, line, col) << "warning:";
}

//-----------------------------------------------------------------------------
QString makeIdentifier(QString identifier)
{
  for (int n = 0; n < identifier.size(); ++n)
    {
    if (!identifier[n].isLetter())
      {
      identifier[n] = '_';
      }
    }
  return identifier;
}

//-----------------------------------------------------------------------------
QString removeAccel(QString text)
{
  // Need our own function because QString::remove is self-modifying
  return text.remove('&');
}

//-----------------------------------------------------------------------------
QString iconName(const ObjectProperties& properties)
{
  if (!properties.contains("icon"))
    {
    return QString();
    }

  QString name = properties.value("icon");
  name = name.mid(name.lastIndexOf('/') + 1);
  return name;
}

//-----------------------------------------------------------------------------
QString codeString(QString value)
{
  if (value.isEmpty())
    {
    return "QString()";
    }
  return '\"' + value + '\"';
}

//-----------------------------------------------------------------------------
void writeGeneratedFileHeader(QTextStream& s, QStringList inFiles)
{
  int w = (inFiles.count() > 1 ? 81 : 80);
  s << '/' << QString(w, '*') << '\n'
    << "** Form generated from reading UI file";
  if (inFiles.count() == 1)
    {
    s << " '" << QFileInfo(inFiles[0]).fileName() << "'\n";
    }
  else
    {
    s << "s:\n";
    foreach (auto const& inFile, inFiles)
      s << "**    '" << QFileInfo(inFile).fileName() << "'\n";
    }
  s << "** Created: " << QDateTime::currentDateTime().toString() << '\n'
    << "**      by: QtExtensions Action Manager Compiler"
    << " version " << QTE_VERSION_STR << '\n'
    << "**\n"
    << "** WARNING! All changes made in this file will be lost"
       " when recompiling UI file";
  if (inFiles.count() > 1)
    {
    s << 's';
    }
  s << "!\n" << QString(w, '*') << "/\n\n";
}

//-----------------------------------------------------------------------------
ObjectProperties extractProperties(const QDomNode& object)
{
  ObjectProperties properties;
  QDomNodeList childNodes = object.childNodes();
  for (int ci = 0; ci < childNodes.count(); ++ci)
    {
    // Get property
    QDomElement element = childNodes.at(ci).toElement();
    if (element.tagName() != "property")
      {
      continue;
      }
    // Get name
    QString propertyName = element.attribute("name");
    if (propertyName.isEmpty())
      {
      continue;
      }
    // Get value
    QString propertyValue;
    QDomNodeList propertyChildNodes = element.childNodes();
    for (int pi = 0; pi < propertyChildNodes.count(); ++pi)
      {
      QDomNode n = propertyChildNodes.at(pi);
      QString tagName = n.toElement().tagName();
      if (tagName == "string" || tagName == "iconset")
        {
        int k = n.childNodes().count() - 1;
        if (k < 0)
          {
          continue;
          }
        propertyValue = n.childNodes().at(k).toText().data();
        break;
        }
      }
    // Add property
    if (!propertyValue.isEmpty())
      {
      properties.insert(propertyName, propertyValue);
      }
    }

  return properties;
}

//-----------------------------------------------------------------------------
bool processUi(QString uiName, QTextStream& managerSource)
{
  QFile uiFile(uiName);
  if (!uiFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    err(uiName) << "unable to open file";
    return false;
    }

  QByteArray data = uiFile.readAll();
  if (!data.startsWith("<?xml"))
    {
    err(uiName) << "file does not appear to be a valid .ui file";
    return false;
    }

  QDomDocument doc;
  int el, ec;
  QString et;
  if (!doc.setContent(data, &et, &el, &ec))
    {
    err(uiName, el, ec) << qPrintable(et);
    err(uiName) << "file is not a valid XML document";
    return false;
    }

  // Get UI class name
  QString uiClassName =
    doc.elementsByTagName("class").at(0).childNodes().at(0).toText().data();

  // Get UI window title
  QString uiWindowTitle;
  QDomNodeList widgetNodes = doc.elementsByTagName("widget");
  for (int wi = 0; wi < widgetNodes.count(); ++wi)
    {
    if (widgetNodes.at(wi).toElement().attribute("name") == uiClassName)
      {
      ObjectProperties properties = extractProperties(widgetNodes.at(wi));
      uiWindowTitle = properties.value("windowTitle");
      break;
      }
    }
  if (uiWindowTitle.isEmpty())
    {
    uiWindowTitle = uiClassName;
    }

  // Get actions
  QDomNodeList actionNodes = doc.elementsByTagName("action");
  if (actionNodes.isEmpty())
    {
    warn(uiName) << "no actions found; no output will be produced";
    return true;
    }
  ActionMap actions;
  ActionGroupMap actionGroups;
  for (int ai = 0; ai < actionNodes.count(); ++ai)
    {
    QDomNode actionNode = actionNodes.at(ai);
    QString actionName = actionNode.toElement().attribute("name");
    ObjectProperties properties = extractProperties(actionNode);
    actions.insert(actionName, properties);
    if (properties.contains("actionGroup"))
      {
      actionGroups.insert(properties["actionGroup"], actionName);
      }
    }

  // Open .ui output file
  QFileInfo uiFileInfo(uiName);
  QString amName = QString("am_%1.h").arg(uiFileInfo.completeBaseName());
  QFile amFile(amName);
  if (!amFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
    err(amName) << "unable to open file for writing";
    return false;
    }

  // Write header
  QTextStream am(&amFile);
  writeGeneratedFileHeader(am, QStringList(uiName));

  // Write inclusion guard and base include headers
  QString inclusionGuardSymbol = makeIdentifier(amName.toUpper());
  am << "#ifndef " << inclusionGuardSymbol << '\n'
     << "#define " << inclusionGuardSymbol << '\n'
     << '\n';
  if (!actionGroups.isEmpty())
    {
    am << "#include <QActionGroup>\n";
    }
  am << "#include <qtActionManager.h>\n"
     << "#include \"ui_" << amName.mid(3) << "\"\n"
     << "\nQT_BEGIN_NAMESPACE\n\n"
     << "class Am_" << uiClassName << "\n{\n"
     << "public:\n";

  // Write action group declarations
  foreach (auto const& actionGroup, actionGroups.uniqueKeys())
    am << "    QActionGroup *" << actionGroup << ";\n";
  am << '\n';

  // Write setupActions method header
  am << "    void setupActions(Ui_" << uiClassName << " &ui, "
                              "QObject *actionGroupParent, "
                              "QSettings &settings)\n"
     << "    {\n";

  // Write code to set up action groups
  if (actionGroups.isEmpty())
    {
    am << "        Q_UNUSED(actionGroupParent);\n\n";
    }
  else
    {
    foreach (auto const& actionGroup, actionGroups.uniqueKeys())
      {
      am << "        " << actionGroup
         << " = new QActionGroup(actionGroupParent);\n";
      foreach (auto const& actionInGroup, actionGroups.values(actionGroup))
        {
        am << "        " << actionGroup << "->addAction(ui."
           << actionInGroup << ");\n";
        }
      am << '\n';
      }
    }

  // Write code to set up actions
  foreach (auto const& iter, qtEnumerate(actions))
    {
    am << "        qtAm->setupAction(settings, ui." << iter.key()
       << ", \"" << uiClassName << '/' << iter.key() << '\"';
    QString icon = iconName(iter.value());
    if (!icon.isEmpty())
      {
      am << ", \"" << icon << '\"';
      }
    am << ");\n";
    }

  // Write setupActions method footer
  am << "    } // setupActions\n\n"
     << "    void setupActions(Ui_" << uiClassName << " &ui, "
                              "QObject *actionGroupParent)\n"
     << "    {\n"
     << "        QSettings settings;\n"
     << "        setupActions(ui, actionGroupParent, settings);\n"
     << "    } // setupActions\n";

  // Write footer
  am << "\n};\n\n"
     << "namespace Am {\n"
     << "    class " << uiClassName
     << ": public Am_" << uiClassName << " {};\n"
     << "} // namespace Am"
     << "\n\nQT_END_NAMESPACE\n\n"
     << "#endif // " << inclusionGuardSymbol << '\n';

  // Write manager code
  QTextStream& ms = managerSource; // alias parameter
  foreach (auto const& iter, qtEnumerate(actions))
    {
    QString icon = iconName(iter.value());
    QString defaultShortcut = iter.value().value("shortcut");
    QString displayGroup = iter.value().value("displayGroup", uiWindowTitle);
    ms << "        registerAction(\""
       << uiClassName << '/' << iter.key() << "\", \"" << displayGroup
       << "\", \"" << removeAccel(iter.value().value("text")) << "\", "
       << codeString(icon) << ", QKeySequence("
       << codeString(defaultShortcut) << ", QKeySequence::PortableText)"
       << ");\n";
    }

  return true;
}

//-----------------------------------------------------------------------------
void usage()
{
  out() << "usage: qte-amc <manager header> <ui file> [ <ui file> ... ]";
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  int retval = 0;

  QCoreApplication app(argc, argv);
  QStringList args = app.arguments().mid(1);

  if (args.count() < 1)
    {
    err() << "no output file specified";
    usage();
    retval = 2;
    }
  else if (args.count() < 2)
    {
    err() << "no input files specified";
    usage();
    retval = 2;
    }
  else
    {
    // Open manager file
    QString managerName = args.takeFirst();
    QFile managerFile(managerName);
    if (!managerFile.open(QIODevice::WriteOnly | QIODevice::Text))
      {
      err(managerName) << "unable to open file";
      retval = 1;
      }
    else
      {
      // Write manager header
      QString managerClass = QFileInfo(managerName).baseName();
      QTextStream ms(&managerFile);
      writeGeneratedFileHeader(ms, args);
      QString inclusionGuardSymbol =
        makeIdentifier(QFileInfo(managerName).fileName().toUpper());
      ms << "#ifndef " << inclusionGuardSymbol << '\n'
         << "#define " << inclusionGuardSymbol << '\n'
         << "\n#include <qtActionManagerDialog.h>\n\n"
         << "class " << managerClass << ": public qtActionManagerDialog\n{\n"
         << "public:\n    " << managerClass
         << "(QWidget *parent = 0, Qt::WindowFlags flags = 0)"
            ": qtActionManagerDialog(parent, flags)\n"
         << "    {\n";

      // Process .ui files
      foreach (auto const& uiFile, args)
        {
        if (!processUi(uiFile, ms))
          {
          retval = 1;
          }
        }

      // Write manager footer
      ms << "    }\n};\n\n"
         << "#endif // " << inclusionGuardSymbol << '\n';
      }

    if (retval)
      {
      out() << "error(s) occurred";
      }
    }

  return retval;
}
