/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

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
