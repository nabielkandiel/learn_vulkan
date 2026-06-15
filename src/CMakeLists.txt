add_subdirectory(common)
add_subdirectory(sdl_tetris)

target_include_directories(sdl_tetris PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_BINARY_DIR}/common
)
