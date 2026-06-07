cmake_minimum_required(VERSION 3.30)
project(nasty_tetris LANGUAGES CXX)

# set this for clangd language server
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# this will get generated into a .h and used in the code
set(PROJECT_ASSET_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/common/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/common/config.h")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(nasty_tetris 
    src/nasty_tetris/main.cpp
    src/nasty_tetris/utility.cpp)

# add includes
target_include_directories(nasty_tetris PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL/include)
target_include_directories(nasty_tetris PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src/common)

# Link to the actual SDL3 library.
target_link_libraries(nasty_tetris PRIVATE SDL3::SDL3)
