# Doxygen functions for the qtExtensions project

option(QTE_BUILD_DOCUMENTATION "Build API documentation" OFF)
cmake_dependent_option(QTE_INSTALL_DOCUMENTATION
  "Install documentation" ON
  "QTE_BUILD_DOCUMENTATION" OFF
)

if(QTE_BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
  add_custom_target(doc)

  # Find qhelpgenerator
  find_program(QHELPGENERATOR_EXECUTABLE qhelpgenerator HINTS ${QT_BINARY_DIR})

  # Find Qt tag file
  find_file(QT4_TAG_FILE
    NAMES qt4.tags qt4.tag qt.tags qt.tag
    PATHS "${QT_DOC_DIR}"
    PATH_SUFFIXES html
  )

  set(QT4_EXTRA_TAG_FILE
    "${CMAKE_BINARY_DIR}/doc/sdk/qt4-extra.tag" CACHE INTERNAL
    "Location of generated file containing additional Doxygen tags for Qt4"
    FORCE
  )
endif()

# Function to create documentation
#   name - partial name of target (generates targets doxygen-${name}, etc.)
#   input_dir - directory containing files to use for documentation
#   ARGN - additional <name>s to use as tag-file dependencies
function(qte_add_documentation name input_dir)
  if(QTE_BUILD_DOCUMENTATION)
    set(tag_files)
    set(tag_targets)

    set(doc_root "${CMAKE_BINARY_DIR}/doc/sdk")
    foreach(tag ${ARGN})
      if("x_${tag}" MATCHES "^x_[Qq][Tt]4?$")
        if(QT4_TAG_FILE)
          list(APPEND tag_files "\"${QT4_TAG_FILE}=${QT_DOC_DIR}/html\"")
        else()
          message(WARNING
            "Documentation for ${name} requested Qt tag file,"
            "but Qt tag file was not found."
          )
        endif()
        set(tag qt4-extra)
        set(tag_file "${QT4_EXTRA_TAG_FILE}")
        set(tag_href "${QT_DOC_DIR}/html")
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
    set(doxyfile_template "${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in")
    set(doxyfile "${doc_dir}/Doxyfile")
    set(doc_css "${CMAKE_CURRENT_LIST_DIR}/qte-dox.css")

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
              -P "${CMAKE_CURRENT_LIST_DIR}/doxygen-script.cmake"
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

    if(QHELPGENERATOR_EXECUTABLE)
      set(qch_file "${doc_root}/${name}.qch")
      add_custom_target(doxygen-${name}-qch
        COMMAND "${QHELPGENERATOR_EXECUTABLE}" index.qhp -o "${qch_file}"
        WORKING_DIRECTORY "${doc_dir}"
        DEPENDS doxygen-${name}
        COMMENT "Creating Qt compressed help for ${name}"
      )
      add_dependencies(doc doxygen-${name}-qch)

      if(QTE_INSTALL_DOCUMENTATION)
        install(FILES "${qch_file}"
          DESTINATION "share/doc/qtExtensions-${QTE_VERSION_STR}/sdk"
          COMPONENT Documentation
        )
      endif()
    endif()
  endif()
endfunction()
