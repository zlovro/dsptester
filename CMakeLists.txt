cmake_minimum_required(VERSION 3.31)
project(dsptester)

set(CMAKE_CXX_STANDARD 26)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(dsptester WIN32
        src/main.cpp
        src/st7920.c
        src/types.h
        src/ui/uicontrol.c
        src/ui/uicontrol.h
        src/ui/effects.c
        src/ui/effects.h
        src/fx/effect_distorsion.c
        src/fx/effect_distorsion.h
        src/fx/effect_eq.c
        src/fx/effect_eq.h
        src/wavstreamer.cpp
        src/wavstreamer.hpp
        src/dsp.c
        src/dsp.h
        src/fx/effect_highpass.h
        src/fx/effect_highpass.c
        src/fx/effect_highpass.h
        src/fx/effect_lowpass.c
        src/fx/effect_lowpass.h
        src/ui/screens/screen_pedalboard.c
        src/ui/screens/screen_pedalboard.h
        src/ui/ui_common.h
)

# Link to the actual SDL3 library.
target_link_libraries(dsptester PRIVATE SDL3::SDL3  winmm)

target_include_directories(dsptester PRIVATE
        vendored
        .
        ASIOSDK
        ASIOSDK/common
        ASIOSDK/host
        ASIOSDK/host/pc

        src
)