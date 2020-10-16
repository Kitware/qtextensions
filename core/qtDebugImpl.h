// This file is part of qtExtensions, and is distributed under the
// OSI-approved BSD 3-Clause License. See top-level LICENSE file or
// https://github.com/Kitware/qtExtensions/blob/master/LICENSE for details.

#ifndef __qtDebugImpl_h
#define __qtDebugImpl_h

#include "qtDebug.h"
#include "qtGlobal.h"

// Override definition from qtDebug.h; see documentation in that file
#undef QTE_EXPORT_DEBUG_AREA
#define QTE_EXPORT_DEBUG_AREA(abi, prefix, area, default_state) \
  QTE_PRIVATE_SINGLETON_WITH_ARGS(qtDebugArea, _##prefix##area, \
                                  (#area, default_state)) \
  abi const qtDebugAreaAccessor& prefix##area = &_##prefix##area

//-----------------------------------------------------------------------------
class qtDebugAreaPrivate;

class QTE_EXPORT qtDebugArea
{
public:
  qtDebugArea(const char* name, bool defaultActive);
  ~qtDebugArea();

protected:
  friend class qtDebug;
  QTE_DECLARE_PRIVATE_PTR(const qtDebugArea);
};

#endif
