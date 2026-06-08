# this will get generated into a .h and used in the code
set(PROJECT_ASSET_PATH "${PROJECT_SOURCE_DIR}/assets/")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h")


