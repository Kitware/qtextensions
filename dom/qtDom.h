// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDom_h
#define __qtDom_h

#include "../core/qtGlobal.h"

class QDomElement;
class QDomNode;
class QString;

template<typename T> class QList;

namespace qtDom
{
  /// Find descendant nodes matching a specified selector.
  ///
  /// This function selects all descendant nodes that match \p selector. Unlike
  /// qtDom::childElements, any descendants, not just immediate children, will
  /// be returned. \p selector is a space-separated list of tag names, that
  /// will be matched in the manner of CSS. The results will be ordered as they
  /// appear in a depth-first traversal of \p root.
  ///
  /// \param root Root node to be searched.
  /// \param selector Tag name, or space separated list of tag names, to match.
  /// \return List of matching DOM elements.
  QTE_EXPORT QList<QDomElement> findElements(const QDomNode& root,
                                             const QString& selector);
}

#endif
