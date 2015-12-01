#------------------------------------------------------------------------------
# Extract first argument into named variable
function(qte_shift_arg variable value)
  set(${variable} "${value}" PARENT_SCOPE)
  set(ARGN "${ARGN}" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
macro(qte_add_slash var)
  if (NOT ${var} MATCHES "^$|.*/$")
    set(${var} "${${var}}/")
  endif()
endmacro()

#------------------------------------------------------------------------------
# Set interface include directories (for build and install) on target
function(qte_library_include_interface target install_prefix)
  qte_add_slash(install_prefix)
  foreach(path ${ARGN})
    target_include_directories(${target} SYSTEM INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${path}>
      $<INSTALL_INTERFACE:${install_prefix}${path}>
    )
  endforeach()
endfunction()

#------------------------------------------------------------------------------
# Mark target(s) for export
function(qte_export_targets)
  set_property(GLOBAL APPEND PROPERTY ${PROJECT_NAME}_EXPORT_TARGETS ${ARGN})
endfunction()

#------------------------------------------------------------------------------
# Install executable target to standard location and set up for export
function(qte_install_executable target component)
  qte_export_targets(${target})
  install(TARGETS ${target}
    EXPORT ${PROJECT_NAME}
    RUNTIME COMPONENT ${component} DESTINATION bin
    BUNDLE  COMPONENT ${component} DESTINATION .
  )
endfunction()

#------------------------------------------------------------------------------
# Install library target(s) to standard location and set up for export
function(qte_install_library_targets)
  qte_export_targets(${ARGN})
  install(TARGETS ${ARGN}
    EXPORT ${PROJECT_NAME}
    RUNTIME       COMPONENT Runtime     DESTINATION bin
    LIBRARY       COMPONENT Runtime     DESTINATION lib${LIB_SUFFIX}
    ARCHIVE       COMPONENT Development DESTINATION lib${LIB_SUFFIX}
  )
endfunction()

#------------------------------------------------------------------------------
# Install headers with subdirectories
function(qte_install_includes destination)
  qte_add_slash(destination)
  foreach (header ${ARGN})
    get_filename_component(subdir "${header}" DIRECTORY)
    qte_add_slash(subdir)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${header}")
      set(header "${CMAKE_CURRENT_BINARY_DIR}/${header}")
    endif()
    install(FILES "${header}"
      COMPONENT Development DESTINATION ${destination}${subdir}
    )
  endforeach()
endfunction()
