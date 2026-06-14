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

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL3_Image EXCLUDE_FROM_ALL)

add_subdirectory(src)

# add warnings for my binaries
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(sdl_tetris INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(sdl_tetris INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wmisleading-indentation
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sdl_tetris INTERFACE
        /W4 # Severe, specific, and informational warnings
        /permissive- # Enforces standards conformance
    )
endif()

# add includes
target_include_directories(sdl_tetris PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL3_Image/include)

# Link to the actual SDL3 library.
target_link_libraries(sdl_tetris PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
