# Build list of required Qt modules
set(Qt5_LINK_MODULES
  Widgets
  Xml
)

set(Qt5_REQUIRED_MODULES
  ${Qt5_LINK_MODULES}
  Designer
  UiPlugin
)

if(QTE_BUILD_DOCUMENTATION)
  list(APPEND Qt5_REQUIRED_MODULES Help)
endif()

# Find Qt5
find_package(Qt5 5.4 COMPONENTS ${Qt5_REQUIRED_MODULES} REQUIRED)

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
macro(qte_wrap_ui)
  qt5_wrap_ui(${ARGN})
endmacro()
macro(qte_add_resources)
  qt5_add_resources(${ARGN})
endmacro()
