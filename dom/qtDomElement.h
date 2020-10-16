// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDomElement_h
#define __qtDomElement_h

#include <QDomElement>

#include "../core/qtGlobal.h"

/// Convenience class for creating DOM documents.
///
/// qtDomElement is a convenience subclass of QDomElement that makes creating
/// DOM documents more convenient. The main feature that qtDomElement provides
/// is the ability to chain element addition. This makes it possible to create
/// simple DOM trees without allocating local variables to every node.
///
/// As an example, consider the following pure-Qt code to create a simple HTML
/// document:
/// \code{.cpp}
/// QDomDocument doc;
/// QDomElement html = doc.createElement("html");
///
/// // Create header
/// QDomElement head = doc.createElement("head");
/// QDomElement contentType = doc.createElement("meta");
/// contentType.setAttribute("http-equiv", "Content-Type")
/// contentType.setAttribute("content", "text/xhtml;charset=UTF-8")
/// head.appendChild(contentType);
/// QDomElement title = doc.createElement("title");
/// QDomText titleText = doc.createText("Example");
/// title.appendChild(titleText);
/// head.appendChild(title);
/// html.appendChild(head);
///
/// // Create body
/// QDomElement body = doc.createElement("body");
/// QDomElement para = doc.createElement("p");
/// para.setAttribute("id", "content");
/// QDomText paraText = doc.createText("Hello, world!");
/// para.appendChild(paraText);
/// body.appendChild(para);
/// html.appendChild(body);
///
/// // Set document content
/// doc.appendChild(html);
/// \endcode
///
/// As you can see, this requires a large number of local variables to be
/// declared and given unique names. It is also inconvenient to add multiple
/// children and/or attributes to a node, as the node variable must be
/// specified each time.
///
/// Using qtDomElement, the above example can be rewritten as follows:
/// \code{.cpp}
/// QDomDocument doc;
/// qtDomElement html(doc, "html");
///
/// // Create header
/// html.add(qtDomElement(doc, "head")
///            .add(qtDomElement(doc, "meta")
///                   .setAttribute("http-equiv", "Content-Type")
///                   .setAttribute("content", "text/xhtml;charset=UTF-8"))
///            .add(qtDomElement(doc, "title").addText("Example")));
///
/// // Create body
/// html.add(qtDomElement(doc, "body")
///            .add(qtDomElement(doc, "p", "content")
///                   .addText("Hello, world!")));
///
/// // Set document content
/// doc.appendChild(html);
/// \endcode
class QTE_EXPORT qtDomElement : public QDomElement
{
public:
  /// Create null element node.
  qtDomElement();

  /// Create a new element.
  ///
  /// This constructs a new element, using the QDomDocument \p doc. The new
  /// element has the tag name \p tagName. If \p id is non-empty, the new
  /// element's "id" attribute is set to \p id.
  explicit qtDomElement(QDomDocument& doc,
                        const QString& tagName,
                        const QString& id = QString());

  /// Construct a qtDomElement from a QDomElement.
  ///
  /// The new element is a copy of \p other. The QDomDocument \p doc is used
  /// if the element needs to create additional nodes, as in addText(), and
  /// should be the same document used to create \p other.
  explicit qtDomElement(QDomDocument& doc, const QDomElement& other);

  /// Append a new node to this element.
  ///
  /// This method appends the specified node to this element's children. Except
  /// for the return value, this is equivalent to QDomNode::appendChild().
  ///
  /// \return Reference to this element.
  qtDomElement& add(const QDomNode&);

  /// Append text to this element.
  ///
  /// This method creates a new QDomText node with text \p text using the
  /// document used to create this element, and appends the new node to this
  /// element's children.
  ///
  /// \return Reference to this element.
  qtDomElement& addText(const QString& text);

  /// Define an attribute.
  ///
  /// Adds an attribute called \p name with value \p value. If an attribute
  /// with the same name exists, its value is replaced by \p value.
  ///
  /// \return Reference to this element.
  qtDomElement& setAttribute(const QString& name, const QString& value);

  /// \{
  /// Define an attribute.
  ///
  /// Adds an attribute called \p name with value \p value. If an attribute
  /// with the same name exists, its value is replaced by \p value.
  ///
  /// The number is formatted according to the current locale.
  ///
  /// \return Reference to this element.
  qtDomElement& setAttribute(const QString& name, int value);
  qtDomElement& setAttribute(const QString& name, uint value);
  qtDomElement& setAttribute(const QString& name, qlonglong value);
  qtDomElement& setAttribute(const QString& name, qulonglong value);
  qtDomElement& setAttribute(const QString& name, float value);
  qtDomElement& setAttribute(const QString& name, double value);
  /// \}

protected:
  /// Reference to the document from which this node was created
  QDomDocument& Document;
};

#endif
