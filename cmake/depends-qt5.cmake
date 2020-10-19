# Build list of required Qt modules
set(Qt5_LINK_MODULES
  Widgets
  Svg
  Xml
)

set(Qt5_REQUIRED_MODULES ${Qt5_LINK_MODULES})

if(QTE_BUILD_DESIGNER_PLUGIN)
  list(APPEND Qt5_REQUIRED_MODULES
    Designer
    UiPlugin
  )
endif()

if(QTE_BUILD_DOCUMENTATION)
  list(APPEND Qt5_REQUIRED_MODULES Help)
endif()

# Find Qt5
find_package(Qt5 5.10 COMPONENTS ${Qt5_REQUIRED_MODULES} REQUIRED)

# Set list of modules to link against
set(QT_LIBRARIES)
foreach(module ${Qt5_LINK_MODULES})
  list(APPEND QT_LIBRARIES Qt5::${module})
endforeach()

# Find qmake
unset(QT_QMAKE_EXECUTABLE CACHE)

get_property(QT_QMAKE_EXECUTABLE
  TARGET Qt5::qmake
  PROPERTY IMPORTED_LOCATION
)

# Define version-compatibility macros
macro(qte_wrap_cpp)
  qt5_wrap_cpp(${ARGN})
endmacro()
