set(QTETEST_LINK_LIBRARIES PRIVATE qtExtensions ${QT_LIBRARIES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Interactive tests
qte_add_test(testDrawers INTERACTIVE
  SOURCES TestDrawers.cpp qtEditableLabel.cpp
  RESOURCES icons.qrc
)

qte_add_test(testExpander INTERACTIVE
  SOURCES TestExpander.cpp
)

qte_add_test(testDoubleSlider INTERACTIVE
  SOURCES TestDoubleSlider.cpp
  UI testDoubleSlider.ui
)

qte_add_test(testSqueezedLabel INTERACTIVE
  SOURCES TestSqueezedLabel.cpp
)

qte_add_test(testColorButton        INTERACTIVE TestColorButton.cpp)
qte_add_test(testConfirmationDialog INTERACTIVE TestConfirmationDialog.cpp)
qte_add_test(testGradientWidget     INTERACTIVE TestGradientWidget.cpp)
qte_add_test(testThrobber           INTERACTIVE TestThrobber.cpp)

# Automated tests
qte_add_test(qtExtensions-Kst testKst
             SOURCES TestKst.cpp ../io/qtKstParser.cpp
             ARGS ${CMAKE_CURRENT_SOURCE_DIR}/testdata.kst
)

qte_add_test(qtExtensions-UiState testUiState TestUiState.cpp)
