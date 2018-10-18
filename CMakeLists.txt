cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (POLICY CMP0063)
  cmake_policy(SET CMP0063 NEW)
endif()

project(qtExtensions)
include(qteVersion.cmake)

#BEGIN general setup

include(CMakeDependentOption)
include(CMakeParseArguments)
include(GenerateExportHeader)

include(cmake/config.cmake)
include(cmake/depends.cmake)
include(cmake/functions.cmake)
include(cmake/wrapping.cmake)
include(cmake/doxygen.cmake)
include(cmake/testing.cmake)

#END general setup

###############################################################################

# BEGIN action manager compiler

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qteVersion.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/qteVersion.h @ONLY)

add_executable(qte-amc tools/qtActionManagerCompiler.cpp)
target_link_libraries(qte-amc ${QT_LIBRARIES})

qte_install_executable(qte-amc Development)

# END action manager compiler

###############################################################################

# BEGIN qtExtensions library sources

set(qtExtensionsSources
    # Core
    core/qtCliArgs.cpp
    core/qtCliOption.cpp
    core/qtCliOptions.cpp
    core/qtDebug.cpp
    core/qtOnce.cpp
    core/qtScopedValueChange.cpp
    core/qtStlUtil.cpp
    core/qtTest.cpp
    core/qtThread.cpp
    core/qtUtil.cpp
    # Util
    util/qtAbstractAnimation.cpp
    util/qtAbstractSetting.cpp
    util/qtActionFactory.cpp
    util/qtActionManager.cpp
    util/qtActionPriorityList.cpp
    util/qtColorScheme.cpp
    util/qtColorUtil.cpp
    util/qtDockController.cpp
    util/qtDockControllerPrivate.cpp
    util/qtGradient.cpp
    util/qtJson.cpp
    util/qtNaturalSort.cpp
    util/qtPrioritizedMenuProxy.cpp
    util/qtPrioritizedToolBarProxy.cpp
    util/qtProcess.cpp
    util/qtScopedSettingsGroup.cpp
    util/qtSettings.cpp
    util/qtStatusForwarder.cpp
    util/qtStatusManager.cpp
    util/qtStatusNotifier.cpp
    util/qtStatusSource.cpp
    util/qtStatusSourcePrivate.cpp
    util/qtUiState.cpp
    # IO
    io/qtKstReader.cpp
    io/qtKstParser.cpp
    io/qtKstSeparator.cpp
    io/qtTemporaryFile.cpp
    # Widgets
    widgets/qtColorButton.cpp
    widgets/qtDoubleSlider.cpp
    widgets/qtDrawer.cpp
    widgets/qtDrawerWidget.cpp
    widgets/qtExpander.cpp
    widgets/qtGradientWidget.cpp
    widgets/qtMenu.cpp
    widgets/qtOverlayWidget.cpp
    widgets/qtProgressWidget.cpp
    widgets/qtSqueezedLabel.cpp
    widgets/qtThrobber.cpp
    # ItemViews
    itemviews/qtAbstractListDelegate.cpp
    itemviews/qtColorButtonItemWidget.cpp
    itemviews/qtComboBoxDelegate.cpp
    itemviews/qtDoubleSpinBoxDelegate.cpp
    itemviews/qtListDelegate.cpp
    itemviews/qtRichTextDelegate.cpp
    itemviews/qtSpinBoxDelegate.cpp
    # Dialogs
    dialogs/qtActionManagerDialog.cpp
    dialogs/qtConfirmationDialog.cpp
    dialogs/qtDoubleInputDialog.cpp
    dialogs/qtGradientEditor.cpp
    # Dom
    dom/qtDom.cpp
    dom/qtDomElement.cpp
    # Sax
    sax/qtSaxNodes.cpp
    sax/qtSaxTraversal.cpp
    sax/qtSaxWriter.cpp
)

set(qtExtensionsUiFiles
    dialogs/qtActionManagerDialog.ui
    dialogs/qtConfirmationDialog.ui
    dialogs/qtGradientEditor.ui
)

set(qtExtensionsInstallHeaders
    # Core
    core/qtCliArgs.h
    core/qtCliOption.h
    core/qtCliOptions.h
    core/qtDebug.h
    core/qtDebugArea.h
    core/qtDebugHelper.h
    core/qtDebugImpl.h
    core/qtEnableSharedFromThis.h
    core/qtEnumerate.h
    core/qtGet.h
    core/qtGlobal.h
    core/qtIndexRange.h
    core/qtMath.h
    core/qtOnce.h
    core/qtPointer.h
    core/qtScopedValueChange.h
    core/qtStlUtil.h
    core/qtTest.h
    core/qtThread.h
    core/qtTransferablePointerArray.h
    core/qtTransferablePointer.h
    core/qtUtil.h
    # Util
    util/qtAbstractAnimation.h
    util/qtAbstractSetting.h
    util/qtActionFactory.h
    util/qtActionManager.h
    util/qtColorScheme.h
    util/qtColorUtil.h
    util/qtDockController.h
    util/qtGradient.h
    util/qtJson.h
    util/qtNaturalSort.h
    util/qtPrioritizedMenuProxy.h
    util/qtPrioritizedToolBarProxy.h
    util/qtProcess.h
    util/qtRand.h
    util/qtScopedSettingsGroup.h
    util/qtSettings.h
    util/qtSettingsImpl.h
    util/qtStatusForwarder.h
    util/qtStatusManager.h
    util/qtStatusNotifier.h
    util/qtStatusSource.h
    util/qtUiState.h
    util/qtUiStateItem.h
    util/qtUtilNamespace.h
    # IO
    io/qtKstReader.h
    io/qtStringStream.h
    io/qtTemporaryFile.h
    # Widgets
    widgets/qtColorButton.h
    widgets/qtDoubleSlider.h
    widgets/qtDrawer.h
    widgets/qtDrawerWidget.h
    widgets/qtExpander.h
    widgets/qtGradientWidget.h
    widgets/qtMenu.h
    widgets/qtOverlayWidget.h
    widgets/qtProgressWidget.h
    widgets/qtSqueezedLabel.h
    widgets/qtThrobber.h
    # ItemViews
    itemviews/qtAbstractListDelegate.h
    itemviews/qtColorButtonItemWidget.h
    itemviews/qtComboBoxDelegate.h
    itemviews/qtDoubleSpinBoxDelegate.h
    itemviews/qtFlagListDelegate.h
    itemviews/qtListDelegate.h
    itemviews/qtNumericTreeWidgetItem.h
    itemviews/qtRichTextDelegate.h
    itemviews/qtSpinBoxDelegate.h
    # Dialogs
    dialogs/qtActionManagerDialog.h
    dialogs/qtConfirmationDialog.h
    dialogs/qtDoubleInputDialog.h
    dialogs/qtGradientEditor.h
    # Dom
    dom/qtDom.h
    dom/qtDomElement.h
    # Sax
    sax/qtSax.h
    sax/qtSaxNamespace.h
    sax/qtSaxNodes.h
    sax/qtSaxTraversal.h
    sax/qtSaxWriter.h
)

# END qtExtensions library build sources

###############################################################################

# BEGIN qtExtensions library build rules

qte_wrap_ui(qtExtensionsUiSources ${qtExtensionsUiFiles})

source_group("Generated" FILES
  ${qtExtensionsUiSources}
)

add_library(${PROJECT_NAME}Headers INTERFACE)
add_library(${PROJECT_NAME}
  ${qtExtensionsSources} ${qtExtensionsUiSources}
)

generate_export_header(${PROJECT_NAME}
  EXPORT_FILE_NAME qtExports.h
  EXPORT_MACRO_NAME QTE_EXPORT
)

if(WIN32)
  set(ONCE_THREAD_LIBRARY)
else()
  set(ONCE_THREAD_LIBRARY -lpthread)
endif()

target_compile_options(${PROJECT_NAME} INTERFACE ${QTE_REQUIRED_CXX_FLAGS})

target_include_directories(${PROJECT_NAME}Headers SYSTEM INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/QtE>
)
qte_library_include_interface(${PROJECT_NAME}Headers include/QtE
  core
  dialogs
  dom
  io
  itemviews
  sax
  util
  widgets
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_NAME}Headers
  ${QT_LIBRARIES}
  PRIVATE
  ${ONCE_THREAD_LIBRARY}
)

if(MSVC AND QTE_QT_VERSION LESS 5)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS /Zc:wchar_t-)
endif()

qte_install_includes(include/QtE ${qtExtensionsInstallHeaders} qtExports.h)
qte_install_library_targets(${PROJECT_NAME} ${PROJECT_NAME}Headers)

# END qtExtensions library build rules

###############################################################################

# BEGIN qtExtensions designer plugin build rules

if(QTE_BUILD_DESIGNER_PLUGIN)
  # Explicitly wrap pure interface headers (that don't have source files)
  qte_wrap_cpp(qtExtensionsDesignerPluginMocSources
    designer/qtColorButtonInterface.h
    designer/qtDoubleSliderInterface.h
    designer/qtExpanderInterface.h
    designer/qtOverlayWidgetInterface.h
    designer/qtProgressWidgetInterface.h
    designer/qtSqueezedLabelInterface.h
    designer/qtThrobberInterface.h
    TARGET ${PROJECT_NAME}DesignerPlugin
  )

  add_library(${PROJECT_NAME}DesignerPlugin MODULE
    designer/qtDesignerPlugin.cpp
    designer/qtDesignerWidgetInterface.cpp
    ${qtExtensionsDesignerPluginMocSources}
  )

  set_property(TARGET ${PROJECT_NAME}DesignerPlugin
    PROPERTY LIBRARY_OUTPUT_DIRECTORY designer
  )

  target_link_libraries(${PROJECT_NAME}DesignerPlugin PRIVATE
    ${PROJECT_NAME}
  )

  if (QTE_QT_VERSION EQUAL 5)
    target_link_libraries(${PROJECT_NAME}DesignerPlugin PRIVATE
      Qt5::Core
      Qt5::Gui
      Qt5::Widgets
      Qt5::Designer
      Qt5::UiPlugin
    )
  endif()

  install(TARGETS ${PROJECT_NAME}DesignerPlugin LIBRARY
    COMPONENT Development
    DESTINATION lib${LIB_SUFFIX}/qte/plugins/designer
  )
endif()

# END qtExtensions designer plugin build rules

###############################################################################

# BEGIN qtExtensions library python wrapping rules

set(qtExtensionsWrapObjects
    # Core
    qtCliOptions
    #qtDebug - not wrapped for now; need to figure out how to wrap areas
    #qtTest - not wrapped for now; needs special handling to pass suites
    qtThread qtBareThread
    # Util
    qtAbstractAnimation
    qtActionFactory
    qtActionManager
    qtDockController
    qtPrioritizedMenuProxy
    qtPrioritizedToolBarProxy
    qtProcess
    qtScopedSettingsGroup
    qtStatusForwarder
    qtStatusManager
    qtStatusNotifier
    qtStatusSource
    # IO
    qtTemporaryFile
    # Widgets
    qtColorButton
    qtDrawerWidget
    qtExpander
    qtGradientWidget
    qtMenu
    qtOverlayWidget
    qtProgressWidget
    qtThrobber
    # ItemViews
    qtAbstractListDelegate
    qtColorButtonItemWidget
    qtComboBoxDelegate
    qtDoubleSpinBoxDelegate
    #qtFlagListDelegate - not wrapped for now; template class, maybe impossible
    qtListDelegate
    qtNumericTreeWidgetItem
    qtRichTextDelegate
    qtSpinBoxDelegate
    # Dialogs
    qtActionManagerDialog
    qtGradientEditor
    # Dom
    qtDomElement
)

qte_wrap_library(${PROJECT_NAME}
  TYPESYSTEM typesystem.xml
  OBJECTS ${qtExtensionsWrapObjects}
  HEADERS ${qtExtensionsInstallHeaders}
  DEPENDS PySide:Gui
)

# END qtExtensions library python wrapping rules

###############################################################################

#BEGIN install rules

# Create package configuration files
configure_file(
  cmake/modules/UseQtExtensions.cmake
  "${${PROJECT_NAME}_BINARY_DIR}/UseQtExtensions.cmake"
  COPYONLY
)

configure_file(
  cmake/${PROJECT_NAME}Config.cmake.in
  "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)

configure_file(
  cmake/${PROJECT_NAME}ConfigVersion.cmake.in
  "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  @ONLY
)

install(FILES
  "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME}
  COMPONENT Development
)

# Create exports file
get_property(export_targets GLOBAL PROPERTY ${PROJECT_NAME}_EXPORT_TARGETS)
export(TARGETS ${export_targets}
  FILE "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}ConfigTargets.cmake"
)

install(EXPORT ${PROJECT_NAME}
  FILE ${PROJECT_NAME}ConfigTargets.cmake
  DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME}
  COMPONENT Development
)

#END install rules

###############################################################################

# BEGIN doxytag helper

add_executable(qte-makedoxytag tools/qtMakeDoxyTags.cpp)
target_link_libraries(qte-makedoxytag ${PROJECT_NAME} ${QT_LIBRARIES})

# List of extra tags (pages) to extract
set(qt_extra_tags
  implicit-sharing
)

# Create dependency list
set(qt_extra_tag_files)
foreach(tag ${qt_extra_tags})
  list(APPEND qt_extra_tag_files "${QT_DOC_DIR_FULL}/${tag}.html")
endforeach()

# Add target to generate extra tags file
add_custom_command(
  OUTPUT "${QT_EXTRA_TAG_FILE}"
  COMMAND qte-makedoxytag -o "${QT_EXTRA_TAG_FILE}" -d "${QT_DOC_DIR_FULL}"
                          -t "${QT_TAG_FILE}" ${qt_extra_tags}
  DEPENDS qte-makedoxytag ${qt_extra_tag_files}
)
add_custom_target(doxygen-qt-extra-tag DEPENDS "${QT_EXTRA_TAG_FILE}")

# END doxytag helper

###############################################################################

install(FILES cmake/modules/UseQtExtensions.cmake
  DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME}
  COMPONENT Development
)

qte_add_documentation(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} Qt)

qte_add_test_subdirectory()
