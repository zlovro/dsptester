cmake_minimum_required(VERSION 3.31)
project(dsptester)

set(CMAKE_CXX_STANDARD 26)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(dsptester WIN32
        main.cpp
        st7920.c
        types.h
)

# Link to the actual SDL3 library.
target_link_libraries(dsptester PRIVATE SDL3::SDL3)

target_include_directories(dsptester PRIVATE
        vendored
        .
)