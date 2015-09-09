#------------------------------------------------------------------------------
# Extract first argument into named variable
function(qte_shift_arg variable value)
  set(${variable} "${value}" PARENT_SCOPE)
  set(ARGN "${ARGN}" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
# Add the standard testing subdirectory, if testing is enabled
function(qte_add_test_subdirectory)
  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()
endfunction()

#------------------------------------------------------------------------------
# Define a test in a standard manner
function(qte_add_test)
  # Extract positional arguments
  qte_shift_arg(_NAME ${ARGN})
  qte_shift_arg(_EXECUTABLE ${ARGN})
  if ("x_${_EXECUTABLE}" STREQUAL "x_INTERACTIVE")
    set(_INTERACTIVE TRUE)
    set(_EXECUTABLE ${_NAME})
  endif()

  # Extract arguments
  set(_opts "SOURCES;MOC_HEADERS;RESOURCES;UI;LINK_LIBRARIES;ARGS")
  cmake_parse_arguments("" "" "" "${_opts}" ${ARGN})
  list(APPEND _SOURCES ${_UNPARSED_ARGUMENTS}) # Use leftover args as sources

  if(NOT TARGET ${_EXECUTABLE})
    # Handle Qt executables
    if(NOT "x_${_MOC_HEADERS}" STREQUAL "x_")
      qt4_wrap_cpp(_MOC_SOURCES ${_MOC_HEADERS})
      list(APPEND _SOURCES ${_MOC_SOURCES})
    endif()
    if(NOT "x_${_UI}" STREQUAL "x_")
      qt4_wrap_ui(_UI_SOURCES ${_UI})
      list(APPEND _SOURCES ${_UI_SOURCES})
    endif()
    if(NOT "x_${_RESOURCES}" STREQUAL "x_")
      qt4_add_resources(_RESOURCE_SOURCES ${_RESOURCES})
      list(APPEND _SOURCES ${_RESOURCE_SOURCES})
    endif()

    # Generate test executable
    add_executable(${_EXECUTABLE} ${_SOURCES})
    target_link_libraries(${_EXECUTABLE}
      PRIVATE
      ${_LINK_LIBRARIES}
      ${QTETEST_LINK_LIBRARIES}
    )
  endif()

  # Add CTest test, if not interactive
  if(NOT _INTERACTIVE)
    add_test(NAME ${_NAME}
             COMMAND $<TARGET_FILE:${_EXECUTABLE}> ${_ARGS})
  endif()
endfunction()

###############################################################################

# Enable testing?
if(BUILD_TESTING)
  enable_testing()
  include(CTest)
endif()
