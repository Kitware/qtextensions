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
  set(QTE_AMC_EXECUTABLE qte-amc) # TODO need a launcher on WIN32 to set PATH
  add_custom_command(OUTPUT ${outfiles} ${outfile}
    COMMAND ${QTE_AMC_EXECUTABLE} ${outfile} ${infiles}
    DEPENDS ${QTE_AMC_EXECUTABLE} ${infiles})
  set(${outvar} ${outfiles} ${outfile} PARENT_SCOPE)
endfunction()
