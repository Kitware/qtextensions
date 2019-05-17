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
  set_source_files_properties(${outfiles} ${outfile}
    PROPERTIES GENERATED TRUE SKIP_AUTOMOC TRUE)

  if (NOT CMAKE_VERSION VERSION_LESS 3.1)
    if (WIN32)
      if(TARGET Qt5::qmake)
        get_property(QT_QMAKE_EXECUTABLE
          TARGET Qt5::qmake
          PROPERTY IMPORTED_LOCATION
        )
      elseif(NOT DEFINED QT_QMAKE_EXECUTABLE)
        message(FATAL_ERROR "Qt must be found before using qte_amc_wrap_ui")
      endif()
      get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

      set(QTE_AMC_ENVIRONMENT
        ${CMAKE_COMMAND} -E env "\"PATH=${QT_BIN_DIR}\\;%PATH%\"")
    else()
      if(QTE_QT_VERSION EQUAL 5)
        if(NOT TARGET Qt5::Core)
          message(FATAL_ERROR "Qt must be found before using qte_amc_wrap_ui")
        endif()
        get_property(QT_QTCORE_LIBRARY TARGET Qt5::Core PROPERTY LOCATION)
      else()
        if(NOT DEFINED QT_QTCORE_LIBRARY)
          message(FATAL_ERROR "Qt must be found before using qte_amc_wrap_ui")
        endif()
      endif()
      get_filename_component(QT_LIB_DIR "${QT_QTCORE_LIBRARY}" DIRECTORY)

      if(APPLE)
        set(QTE_AMC_ENVIRONMENT
          ${CMAKE_COMMAND} -E env "\"DYLD_FALLBACK_LIBRARY_PATH=${QT_LIB_DIR}:\${DYLD_FALLBACK_LIBRARY_PATH}\"")
      else()
        set(QTE_AMC_ENVIRONMENT
          ${CMAKE_COMMAND} -E env "\"LD_LIBRARY_PATH=${QT_LIB_DIR}:\${LD_LIBRARY_PATH}\"")
      endif()
    endif()
  endif()

  set(QTE_AMC_EXECUTABLE $<TARGET_FILE:qte-amc>)

  add_custom_command(OUTPUT ${outfiles} ${outfile}
    COMMAND ${QTE_AMC_ENVIRONMENT}
            ${QTE_AMC_EXECUTABLE} ${outfile} ${infiles}
    DEPENDS ${QTE_AMC_EXECUTABLE} ${infiles})
  set(${outvar} ${outfiles} ${outfile} PARENT_SCOPE)
endfunction()
