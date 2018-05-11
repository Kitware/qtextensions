set(QTE_QT_VERSION "4" CACHE STRING "Qt version to use")

set_property(CACHE QTE_QT_VERSION PROPERTY STRINGS 4 5)

if(NOT (QTE_QT_VERSION VERSION_EQUAL "4" OR QTE_QT_VERSION VERSION_EQUAL "5"))
  message(FATAL_ERROR "Expected value for QTE_QT_VERSION is either '4' or '5'")
endif()
