// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

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
