# Find Qt4
set(QT_USE_QTXML TRUE)
find_package(Qt4 4.8.0 REQUIRED)
include(${QT_USE_FILE})

# Define version-compatibility macros
macro(qte_wrap_cpp)
  qt4_wrap_cpp(${ARGN})
endmacro()
macro(qte_wrap_ui)
  qt4_wrap_ui(${ARGN})
endmacro()
macro(qte_add_resources)
  qt4_add_resources(${ARGN})
endmacro()
