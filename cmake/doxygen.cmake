# Doxygen functions for the qtExtensions project

cmake_dependent_option(QTE_INSTALL_DOCUMENTATION
  "Install documentation" ON
  "QTE_BUILD_DOCUMENTATION" OFF
)

###############################################################################

#------------------------------------------------------------------------------
# Query a value from qmake
function(qte_query_qmake var result)
  execute_process(COMMAND ${QT_QMAKE_EXECUTABLE} -query ${var}
    RESULT_VARIABLE return_code
    OUTPUT_VARIABLE output
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
  if(NOT return_code)
    file(TO_CMAKE_PATH "${output}" output)
    set(${result} ${output} PARENT_SCOPE)
  endif()
endfunction()

#------------------------------------------------------------------------------
# Create documentation
#   name - partial name of target (generates targets doxygen-${name}, etc.)
#   input_dir - directory containing files to use for documentation
#   ARGN - additional <name>s to use as tag-file dependencies
function(qte_add_documentation name input_dir)
  if(QTE_BUILD_DOCUMENTATION)
    set(tag_files)
    set(tag_targets)

    set(doc_root "${CMAKE_BINARY_DIR}/doc")
    foreach(tag ${ARGN})
      if("x_${tag}" MATCHES "^x_[Qq][Tt][45]?$")
        if(QT_TAG_FILE)
          list(APPEND tag_files "\"${QT_TAG_FILE}=${QT_DOC_DIR_FULL}\"")
        else()
          message(WARNING
            "Documentation for ${name} requested Qt tag file, "
            "but Qt tag file was not found."
          )
        endif()
        set(tag qt-extra)
        set(tag_file "${QT_EXTRA_TAG_FILE}")
        set(tag_href "${QT_DOC_DIR_FULL}")
      else()
        set(tag_file "${doc_root}/${tag}/${tag}.tag")
        set(tag_href "../${tag}")
      endif()
      list(APPEND tag_files "\"${tag_file}=${tag_href}\"")
      list(APPEND tag_targets doxygen-${tag}-tag)
    endforeach()
    string(REPLACE ";" " " tag_files "${tag_files}")

    if(TARGET ${name})
      get_target_property(DOXYGEN_INCLUDE_PATHS
        ${name} INCLUDE_DIRECTORIES
      )
    else()
      get_directory_property(DOXYGEN_INCLUDE_PATHS
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        INCLUDE_DIRECTORIES
      )
    endif()
    string(REPLACE ";" " " DOXYGEN_INCLUDE_PATHS "${DOXYGEN_INCLUDE_PATHS}")

    set(doc_dir "${doc_root}/${name}")
    set(doxyfile_template "${QTE_DOCUMENTATION_RESOURCES}/Doxyfile.in")
    set(doxyfile "${doc_dir}/Doxyfile")
    set(doc_css "${QTE_DOCUMENTATION_RESOURCES}/qte-dox.css")

    add_custom_command(
      OUTPUT "${doc_dir}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${doc_dir}"
      COMMENT "Creating documentation directory for ${name}"
    )
    add_custom_command(
      OUTPUT "${doxyfile}.tag" "${doxyfile}.html"
      COMMAND "${CMAKE_COMMAND}"
              -D "DOXYGEN_TEMPLATE=${doxyfile_template}"
              -D "DOXYGEN_PROJECT_SOURCE_DIR=${input_dir}"
              -D "DOXYGEN_DOCUMENTATION_OUTPUT_PATH=${doc_dir}"
              -D "DOXYGEN_OUTPUT_FILE=${doxyfile}"
              -D "DOXYGEN_PROJECT_NAME=${name}"
              -D "DOXYGEN_TAG_FILES=${tag_files}"
              -D "DOXYGEN_EXCLUDE_PATTERNS=${DOXYGEN_EXCLUDE_PATTERNS}"
              -D "DOXYGEN_INCLUDE_PATHS=${DOXYGEN_INCLUDE_PATHS}"
              -D "DOXYGEN_EXTRA_STYLESHEET=${doc_css}"
              -P "${QTE_DOCUMENTATION_RESOURCES}/doxygen-script.cmake"
      DEPENDS "${doc_dir}" "${doxyfile_template}"
      WORKING_DIRECTORY "${doc_dir}"
      COMMENT "Generating Doxyfile for ${name}"
    )
    add_custom_target(doxygen-${name}-tag DEPENDS "${doxyfile}.tag")
    add_custom_command(
      TARGET doxygen-${name}-tag
      COMMAND "${DOXYGEN_EXECUTABLE}" "${doxyfile}.tag"
      WORKING_DIRECTORY "${doc_dir}"
      COMMENT "Creating tags for ${name}"
    )
    add_custom_target(doxygen-${name}
      DEPENDS ${tag_targets} "${doxyfile}.html" "${doc_css}"
      COMMAND "${DOXYGEN_EXECUTABLE}" "${doxyfile}.html"
      WORKING_DIRECTORY "${doc_dir}"
      COMMENT "Creating documentation for ${name}"
    )
    add_dependencies(doc doxygen-${name})

    if(QTE_INSTALL_DOCUMENTATION)
      install(
        DIRECTORY "${doc_dir}"
        DESTINATION "share/doc/qtExtensions-${QTE_VERSION_STR}/${name}"
        COMPONENT Documentation
      )
    endif()

    if(QT_QHELPGENERATOR_EXECUTABLE)
      set(qch_file "${doc_root}/${name}.qch")
      add_custom_target(doxygen-${name}-qch
        COMMAND ${QT_QHELPGENERATOR_EXECUTABLE} index.qhp -o "${qch_file}"
        WORKING_DIRECTORY "${doc_dir}"
        DEPENDS doxygen-${name}
        COMMENT "Creating Qt compressed help for ${name}"
      )
      add_dependencies(doc doxygen-${name}-qch)

      if(QTE_INSTALL_DOCUMENTATION)
        install(FILES "${qch_file}"
          DESTINATION "share/doc/qtExtensions-${QTE_VERSION_STR}"
          COMPONENT Documentation
        )
      endif()
    endif()
  endif()
endfunction()

###############################################################################

set(QTE_DOCUMENTATION_RESOURCES ${CMAKE_CURRENT_LIST_DIR})

if(QTE_BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
  add_custom_target(doc)

  # Find qhelpgenerator
  if(TARGET Qt5::qhelpgenerator)
    get_property(QT_QHELPGENERATOR_EXECUTABLE
      TARGET Qt5::qhelpgenerator
      PROPERTY IMPORTED_LOCATION
    )
  else()
    find_program(QT_QHELPGENERATOR_EXECUTABLE
      qhelpgenerator
      HINTS ${QT_BINARY_DIR})
  endif()

  if (QTE_QT_VERSION EQUAL 4)
    # Find Qt4 tag file
    find_file(QT_TAG_FILE
      NAMES qt4.tags qt4.tag qt.tags qt.tag
      PATHS "${QT_DOC_DIR}"
      PATH_SUFFIXES html
    )
  else()
    # Find Qt5::QtCore tag file
    if (NOT QT_DOC_DIR)
      qte_query_qmake(QT_INSTALL_DOCS qt_doc_dir)
      set(QT_DOC_DIR
        ${qt_doc_dir}
        CACHE PATH "The location of the Qt docs" FORCE
      )
    endif()

    find_file(QT_TAG_FILE
      NAMES qtcore.tags
      PATHS "${QT_DOC_DIR}"
      PATH_SUFFIXES qtcore
    )
  endif()
  get_filename_component(QT_DOC_DIR_FULL
    "${QT_TAG_FILE}" DIRECTORY
  )

  set(QT_EXTRA_TAG_FILE
    "${CMAKE_BINARY_DIR}/doc/qt-extra.tag" CACHE INTERNAL
    "Location of generated file containing additional Doxygen tags for Qt"
    FORCE
  )
endif()
