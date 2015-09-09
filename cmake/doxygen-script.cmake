# Create configuration for generating tag file
set(DOXYGEN_OUTPUT_TAG_FILE
  "${DOXYGEN_DOCUMENTATION_OUTPUT_PATH}/${DOXYGEN_PROJECT_NAME}.tag"
)
set(GENERATE_HTML NO)

configure_file(
  "${DOXYGEN_TEMPLATE}"
  "${DOXYGEN_OUTPUT_FILE}.tag"
  @ONLY
)

# Create configuration for generating HTML
set(DOXYGEN_OUTPUT_TAG_FILE)
set(GENERATE_HTML YES)

configure_file(
  "${DOXYGEN_TEMPLATE}"
  "${DOXYGEN_OUTPUT_FILE}.html"
  @ONLY
)
