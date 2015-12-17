set(QTETEST_LINK_LIBRARIES PRIVATE qtExtensions ${QT_LIBRARIES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Interactive tests
qte_add_test(testDrawers INTERACTIVE
  SOURCES TestDrawers.cpp qtEditableLabel.cpp
  MOC_HEADERS TestDrawers.h qtEditableLabel.h
  RESOURCES icons.qrc
)

qte_add_test(testExpander INTERACTIVE
  SOURCES TestExpander.cpp
  MOC_HEADERS TestExpander.h
)

qte_add_test(testDoubleSlider INTERACTIVE
  SOURCES TestDoubleSlider.cpp
  MOC_HEADERS TestDoubleSlider.h
  UI testDoubleSlider.ui
)

qte_add_test(testSqueezedLabel INTERACTIVE
  SOURCES TestSqueezedLabel.cpp
  MOC_HEADERS TestSqueezedLabel.h
)

qte_add_test(testColorButton        INTERACTIVE TestColorButton.cpp)
qte_add_test(testConfirmationDialog INTERACTIVE TestConfirmationDialog.cpp)
qte_add_test(testGradientEditor     INTERACTIVE TestGradientEditor.cpp)
qte_add_test(testGradientWidget     INTERACTIVE TestGradientWidget.cpp)
qte_add_test(testThrobber           INTERACTIVE TestThrobber.cpp)

# Automated tests
qte_add_test(qtExtensions-Kst testKst
             SOURCES TestKst.cpp ../io/qtKstParser.cpp
             ARGS ${CMAKE_CURRENT_SOURCE_DIR}/testdata.kst
)

qte_add_test(qtExtensions-UiState testUiState TestUiState.cpp)
