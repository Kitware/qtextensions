// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#include "qtDom.h"

#include "../core/qtIndexRange.h"

#include <QDomElement>
#include <QList>
#include <QStringList>

namespace qtDom
{

//-----------------------------------------------------------------------------
QList<QDomElement> findElements(
  QList<QDomElement>& result, const QDomNode& root,
  const QStringList& selectors, int selectorIndex = 0)
{
  const QString selector = selectors[selectorIndex];

  // Test root for match
  if (root.isElement() && root.toElement().tagName() == selector)
    {
    if (selectors.count() == selectorIndex + 1)
      {
      // Node is a terminal match; add to result list
      result.append(root.toElement());
      }
    else
      {
      // Node is an acceptable part of a match chain; continue search against
      // children with shortened selector list
      findElements(result, root, selectors, selectorIndex + 1);
      }
    }

  // Iterate over child nodes
  const QDomNodeList childNodes = root.childNodes();
  foreach (auto const i, qtIndexRange(childNodes.count()))
    {
    findElements(result, childNodes.at(i), selectors, selectorIndex);
    }

  return result;
}

//-----------------------------------------------------------------------------
QList<QDomElement> findElements(const QDomNode& root,
                                const QString& selector)
{
  const QStringList selectors = selector.split(' ', Qt::SkipEmptyParts);
  QList<QDomElement> result;
  return findElements(result, root, selectors);
}

} // namespace qtDom
