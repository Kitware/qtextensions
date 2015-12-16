/*ckwg +5
 * Copyright 2015 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include <QTextStream>

#include "../core/qtCliArgs.h"
#include "../dom/qtDom.h"
#include "../dom/qtDomElement.h"
#include "qteVersion.h"

//-----------------------------------------------------------------------------
QString copyChildText(qtDomElement& out, QDomDocument& doc,
                      const QDomElement& in, const QString& elementName)
{
  const QDomElement& child = in.firstChildElement(elementName);
  if (!child.isNull())
    {
    const QString& text = child.text();
    out.add(qtDomElement(doc, elementName).addText(text));
    return text;
    }

  return QString();
}

//-----------------------------------------------------------------------------
void parseTags(const QString& filename, QDomNode& root, QDomDocument& doc)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
    {
    qWarning().nospace()
      << "Error opening tag file " << filename << ": " << file.errorString();
    qWarning() << "Tag conversion will not be performed";
    return;
    }

  QDomDocument tags;
  QString error;
  int el, ec;
  if (!tags.setContent(&file, &error, &el, &ec))
    {
    qWarning().nospace()
      << qPrintable(filename) << ':' << el << ':' << ec
      << ": " << qPrintable(error);
    return;
    }

  // Iterate over compounds in the input tag DOM
  foreach (auto const& iscope, qtDom::findElements(tags, "compound"))
    {
    // Create and populate a scope node from the input scope node
    qtDomElement oscope(doc, "compound");
    bool empty = true;
    oscope.setAttribute("kind", iscope.attribute("kind"));
    copyChildText(oscope, doc, iscope, "name");
    const QString anchorFile = copyChildText(oscope, doc, iscope, "filename");

    // Iterate over members in the input scope node
    foreach (auto const& itag, qtDom::findElements(iscope, "member"))
      {
      // See if this looks like an enum value; all tags with names and no kind
      // are considered to be enum values, which may not be entirely accurate
      // but is the best we can do from Qt's deficient tag file format
      if (itag.hasAttribute("name") && !itag.hasAttribute("kind"))
        {
        // Looks like an enum value; create a properly formatted tag for the
        // output tag file
        qtDomElement otag(doc, "member");
        otag.setAttribute("kind", "enumvalue");
        otag.add(qtDomElement(doc, "type").addText("@"))
            .add(qtDomElement(doc, "name").addText(itag.attribute("name")))
            .add(qtDomElement(doc, "anchorfile").addText(anchorFile));
        copyChildText(otag, doc, itag, "anchor");
        oscope.add(otag);
        empty = false;
        }
      }

    // If we found any tags for this scope, add the scope to the output DOM
    if (!empty)
      {
      root.appendChild(oscope);
      }
    }
}

//-----------------------------------------------------------------------------
QDomNode parseInput(
  const QString& tag, const QString& filename, QDomDocument& doc)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
    {
    qWarning().nospace()
      << "Error opening page " << filename << ": " << file.errorString();
    return QDomNode();
    }

  QDomDocument html;
  QString error;
  int el, ec;
  if (!html.setContent(&file, &error, &el, &ec))
    {
    qWarning().nospace()
      << qPrintable(filename) << ':' << el << ':' << ec
      << ": " << qPrintable(error);
    return QDomNode();
    }

  QList<QDomElement> title = qtDom::findElements(html, "h1");
  if (title.isEmpty())
    {
    qWarning() << "Could not find page title in" << filename;
    return QDomNode();
    }

  // TODO extract sections?
  // <docanchor file="${tag}">${anchor-name}</docanchor>

  qtDomElement node(doc, "compound");
  node.setAttribute("kind", "page");
  node.add(qtDomElement(doc, "name").addText(tag))
      .add(qtDomElement(doc, "title").addText(title[0].text()))
      .add(qtDomElement(doc, "filename").addText(tag));

  return node;
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  qtCliArgs args(argc, argv);

  // Set up options
  qtCliOptions options;
  options.add("o <file>", "Name of output tag file to create",
              qtCliOption::Required);
  options.add("d <dir>", "Location of Qt HTML documentation",
              qtCliOption::Required);
  args.addOptions(options);

  qtCliOptions nargs;
  nargs.add("tags", "List of tags (page names) to extract",
            qtCliOption::NamedList);
  args.addNamedArguments(nargs);

  // Parse arguments
  args.parseOrDie();
  const QString outname = args.value("o");
  const QString docdir = args.value("d");

  // Create tagfile root
  QDomDocument tags;
  static const char* const processing =
    "version='1.0' encoding='utf-8' standalone='yes' ";
  tags.appendChild(tags.createProcessingInstruction("xml", processing));
  QDomElement root = tags.createElement("tagfile");

  // Extract enum values
  qDebug() << "Extracting enum values";
  parseTags(docdir + "/qt.tags", root, tags);

  // Extract tags
  int errors = 0;
  foreach (auto const& tag, args.values("tags"))
    {
    qDebug() << "Extracting tag" << tag;
    QDomNode node = parseInput(tag, docdir + '/' + tag + ".html", tags);
    if (node.isNull())
      {
      ++errors;
      continue;
      }
    root.appendChild(node);
    }

  if (errors)
    {
    qWarning() << "Error(s) occurred; no output was written";
    return 1;
    }

  tags.appendChild(root);

  // Write output tag file
  QFile out(outname);
  if (!out.open(QIODevice::WriteOnly))
    {
    qWarning().nospace()
      << "Error opening output file " << outname << ": " << out.errorString();
    return 1;
    }

  QTextStream stream(&out);
  stream.setCodec("UTF-8");
  stream << tags.toString(2);

  return 0;
}
