include(CheckCXXCompilerFlag)

#------------------------------------------------------------------------------
# Execute a try_compile test with verbose status
function(qte_verbose_try_compile variable file message)
  message(STATUS "Checking if ${message}...")
  try_compile(${variable}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/configcheck/${file}
  )
  if(${variable})
    message(STATUS "Checking if ${message} - yes")
  else()
    message(STATUS "Checking if ${message} - no")
  endif()
endfunction()

#------------------------------------------------------------------------------
# Add first of specified flags that is supported by the compiler
function(qte_add_cxx_flags_priority)
  string(REPLACE " " ";" initial_flags "${CMAKE_CXX_FLAGS}")
  foreach(flag ${ARGN})
    list(FIND initial_flags ${flag} FLAG_INDEX)
    if(NOT FLAG_INDEX EQUAL -1)
      set(ADDED_FLAG "${flag}" PARENT_SCOPE)
      return()
    endif()
    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" varname "${flag}")
    check_cxx_compiler_flag("${flag}" CMAKE_CXX_COMPILER_SUPPORTS_${varname})
    if(CMAKE_CXX_COMPILER_SUPPORTS_${varname})
      add_compile_options("${flag}")
      set(ADDED_FLAG "${flag}" PARENT_SCOPE)
      return()
    endif()
  endforeach()
  set(ADDED_FLAG "" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
# Add specified flags, if supported by the compiler
function(qte_add_cxx_flags)
  foreach(flag ${ARGN})
    qte_add_cxx_flags_priority("${flag}")
  endforeach()
endfunction()

#------------------------------------------------------------------------------
# Test for required C++ feature
function(qte_test_cxx_feature feature message)
  qte_verbose_try_compile(${feature}_WORKS ${feature}.cpp "${message}")
  if(NOT ${feature}_WORKS)
    message(FATAL_ERROR
      "A required compiler feature is missing. "
      "Try adjusting your CMAKE_CXX_FLAGS or using a newer compiler."
    )
  endif()
endfunction()

###############################################################################

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(QTE_QT_VERSION "4" CACHE STRING "Qt version to use")
set_property(CACHE QTE_QT_VERSION PROPERTY STRINGS 4 5)

cmake_dependent_option(
  QTE_ENABLE_PYTHON "Enable qtExtensions Python (PySide) bindings" OFF
  "QTE_QT_VERSION EQUAL 4" OFF # TODO Support PySide2
)

option(QTE_BUILD_DOCUMENTATION "Build API documentation" OFF)
option(QTE_BUILD_DESIGNER_PLUGIN "Build plugin for Designer" ON)

# Use RPATH on OS/X
if(APPLE)
  set(CMAKE_MACOSX_RPATH TRUE)
endif()

# Set compiler flags for exports (e.g. -fvisibility=hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Set extra compiler flags
if(MSVC)
  # Determine what flags (if any) are needed to prevent the compiler from lying
  # about it's level of C++ language support (i.e. __cplusplus)
  qte_add_cxx_flags_priority(-Zc:__cplusplus)
  set(QTE_REQUIRED_CXX_FLAGS "${ADDED_FLAG}" CACHE INTERNAL "")
else()
  set(QTE_REQUIRED_CXX_FLAGS "" CACHE INTERNAL "")

  # Turn on extra warnings if requested
  option(QTE_EXTRA_WARNINGS "Enable extra warnings" ON)
  if(QTE_EXTRA_WARNINGS)
    qte_add_cxx_flags(
      -Wall -Wextra -Wunused -Wuninitialized -Wctor-dtor-privacy
      -Wcast-align -Wlogical-op -Wpointer-arith
    )
  endif()
  # Turn on additional warnings-as-errors
  qte_add_cxx_flags(
    # Missing 'return'
    -Werror=return-type
    # Improper use of 'virtual'
    -Werror=non-virtual-dtor -Werror=overloaded-virtual
    # Casting away 'const' without const_cast (i.e. via a C-style cast)
    -Werror=cast-qual
    # Use of ill-formed constructs
    -Werror=narrowing
    # Local and member variable initialization issues
    -Werror=init-self -Werror=reorder
  )
endif()

# Test for required compiler features
qte_test_cxx_feature(
  auto "compiler supports 'auto' as a type declaration")
qte_test_cxx_feature(
  brace-init "compiler supports type elision and uniform initialization")
qte_test_cxx_feature(
  defaulted-ctor "compiler supports explicitly defaulted constructors")
qte_test_cxx_feature(
  nested-template "compiler does not require space between template '>'s")
