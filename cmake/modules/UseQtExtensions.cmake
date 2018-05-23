# - Use Module for QtExtensions
# Provides CMake macros to use qte-amc.

# qte_amc_wrap_ui(outfiles_var dialog_class_name input_ui_file ... )
function(qte_amc_wrap_ui outvar name)
  set(infiles)
  set(outfiles)
  foreach(it ${ARGN})
    get_filename_component(outfile ${it} NAME_WE)
    get_filename_component(infile ${it} ABSOLUTE)
    list(APPEND infiles "${infile}")
    list(APPEND outfiles "${CMAKE_CURRENT_BINARY_DIR}/am_${outfile}.h")
  endforeach()
  set(outfile "${CMAKE_CURRENT_BINARY_DIR}/${name}.h")
  set_source_files_properties(${outfiles} ${outfile} PROPERTIES GENERATED TRUE)

  if (NOT CMAKE_VERSION VERSION_LESS 3.1)
    if(TARGET Qt5::Core)
      get_property(_Qt5_Core_LOCATION TARGET Qt5::Core PROPERTY LOCATION)
      get_filename_component(QT_BIN_DIR "${_Qt5_Core_LOCATION}" PATH)
    elseif(WIN32 AND DEFINED QT_QMAKE_EXECUTABLE)
      get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    elseif(APPLE AND DEFINED QT_QTCORE_LIBRARY)
      get_filename_component(QT_BIN_DIR "${QT_QTCORE_LIBRARY}" DIRECTORY)
    else()
      message(FATAL_ERROR "Qt must be found before using qte_amc_wrap_ui")
    endif()

    if(WIN32)
      set(QTE_AMC_ENVIRONMENT
        ${CMAKE_COMMAND} -E env "\"PATH=${QT_BIN_DIR}\\;%PATH%\"")
    elseif (APPLE)
      set(QTE_AMC_ENVIRONMENT
        ${CMAKE_COMMAND} -E env "\"DYLD_FALLBACK_LIBRARY_PATH=${QT_BIN_DIR}:\${DYLD_FALLBACK_LIBRARY_PATH}\"")
    else()
      # TODO need to set library path?
      set(QTE_AMC_ENVIRONMENT)
    endif()
  endif()

  set(QTE_AMC_EXECUTABLE $<TARGET_FILE:qte-amc>)

  add_custom_command(OUTPUT ${outfiles} ${outfile}
    COMMAND ${QTE_AMC_ENVIRONMENT}
            ${QTE_AMC_EXECUTABLE} ${outfile} ${infiles}
    DEPENDS ${QTE_AMC_EXECUTABLE} ${infiles})
  set(${outvar} ${outfiles} ${outfile} PARENT_SCOPE)
endfunction()
