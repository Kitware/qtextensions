include(CheckCXXCompilerFlag)

#------------------------------------------------------------------------------
# Execute a try_compile test with verbose status
function(qte_verbose_try_compile variable file message)
  message(STATUS "Checking if ${message}...")
  try_compile(${variable}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/configcheck/${file}
    CMAKE_FLAGS "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
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
  string(REPLACE " " ";" initial_flags ${CMAKE_CXX_FLAGS})
  foreach(flag ${ARGN})
    list(FIND initial_flags ${flag} FLAG_INDEX)
    if(NOT FLAG_INDEX EQUAL -1)
      set(ADDED_FLAG "${flag}" PARENT_SCOPE)
      return()
    endif()
    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" varname "${flag}")
    check_cxx_compiler_flag("${flag}" CMAKE_CXX_COMPILER_SUPPORTS_${varname})
    if(CMAKE_CXX_COMPILER_SUPPORTS_${varname})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
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
option(QTE_ENABLE_PYTHON "Enable qtExtensions Python (PySide) bindings" OFF)

# Set compiler flags for exports (e.g. -fvisibility=hidden)
add_compiler_export_flags()

# Set extra compiler flags
if(NOT MSVC)
  # Determine what flags (if any) are needed for required C++ language support
  # Note: MSVC always uses latest known C++ extensions
  qte_add_cxx_flags_priority(-std=c++11 -std=c++0x)
  set(QTE_REQUIRED_CXX_FLAGS "${ADDED_FLAG}" CACHE INTERNAL "")

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
