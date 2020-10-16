// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtUtilNamespace_h
#define __qtUtilNamespace_h

#include <QtGlobal>

namespace qtUtil
{
  /// Separator behavior for prioritized action container proxies.
  ///
  /// This describes the behavior of prioritized action container proxies with
  /// respect to inserting separators around inserted actions.
  ///
  /// \sa qtPrioritizedMenuProxy, qtPrioritizedToolBarProxy
  enum SeparatorBehaviorFlag
    {
    /// Add separator before inserted actions
    AddSeparatorBefore = 0x1,
    /// Add separator after inserted actions
    AddSeparatorAfter = 0x2,
    /// Do not add separators
    NoSeparators = 0x0
    };
  Q_DECLARE_FLAGS(SeparatorBehavior, SeparatorBehaviorFlag)
}

Q_DECLARE_OPERATORS_FOR_FLAGS(qtUtil::SeparatorBehavior)

#endif
