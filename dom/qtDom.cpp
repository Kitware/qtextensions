/*ckwg +5
 * Copyright 2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <QDomElement>
#include <QList>
#include <QStringList>

#include "qtDom.h"

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
  for (int i = 0, k = childNodes.count(); i < k; ++i)
    {
    findElements(result, childNodes.at(i), selectors, selectorIndex);
    }

  return result;
}

//-----------------------------------------------------------------------------
QList<QDomElement> findElements(const QDomNode& root,
                                const QString& selector)
{
  const QStringList selectors = selector.split(' ', QString::SkipEmptyParts);
  QList<QDomElement> result;
  return findElements(result, root, selectors);
}

} // namespace qtDom
