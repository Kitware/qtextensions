if(QTE_QT_VERSION EQUAL 5)
  include(${CMAKE_CURRENT_LIST_DIR}/depends-qt5.cmake)
else()
  message(FATAL_ERROR
    "Unsupported QTE_QT_VERSION ${QTE_QT_VERSION}. "
    "Supported versions are '5'."
  )
endif()

# if(QTE_QT_VERSION EQUAL 4) # TODO Support PySide2
#   if(QTE_ENABLE_PYTHON)
#     find_package(PythonInterp REQUIRED VERSION 2.7)
#     find_package(PythonLibs REQUIRED)
#     find_package(Shiboken REQUIRED 1.2)
#     find_package(PySide REQUIRED 1.2)
#   endif()
# endif()
