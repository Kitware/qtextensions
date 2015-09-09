/*ckwg +5
 * Copyright 2013 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef __qtDebugArea_h
#define __qtDebugArea_h

/// \file

class qtDebugArea;

/// Accessor type for a debug area.
///
/// This type defines an accessor for a debug area, representing both the type
/// of the externally declared debug area variables, and the type taken by
/// ::qtDebug's constructor. It is assignable and copyable (both operations are
/// inexpensive and reentrant), but should be considered opaque. New instances
/// should be initialized to an existing instance, a registered debug area, or
/// qtDebug::InvalidArea.
typedef qtDebugArea* (*qtDebugAreaAccessor)();

#endif
