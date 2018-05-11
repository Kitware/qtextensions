include(cmake/qtVersion.cmake)

if(QTE_QT_VERSION VERSION_GREATER 4)
  set(Qt5_MODULES
    Designer
    UiPlugin
    Widgets
    Xml
  )
  find_package(Qt5 COMPONENTS ${Qt5_MODULES} REQUIRED)
  set(QT_LIBRARIES )
  foreach(module ${Qt5_MODULES})
    list(APPEND QT_LIBRARIES Qt5::${module})
  endforeach()
else()
  set(QT_USE_QTXML TRUE)
  find_package(Qt4 4.8.0 REQUIRED)
  include(${QT_USE_FILE})
endif()

if(QTE_ENABLE_PYTHON)
  find_package(PythonInterp REQUIRED VERSION 2.7)
  find_package(PythonLibs REQUIRED)
  find_package(Shiboken REQUIRED 1.2)
  find_package(PySide REQUIRED 1.2)
endif()
