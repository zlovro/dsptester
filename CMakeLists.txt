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
        src/fx/effects.c
        src/fx/effects.h
        src/fx/effect_distorsion.c
        src/fx/effect_distorsion.h
        src/fx/effect_eq.c
        src/fx/effect_eq.h
        src/wavstreamer.cpp
        src/wavstreamer.hpp
        src/dsp.c
        src/dsp.h
        src/ui/screens/screen_pedalboard.c
        src/ui/screens/screen_pedalboard.h
        src/ui/ui_common.h
        src/fx/algo/fxalgo_xpass.c
        src/fx/algo/fxalgo_xpass.h
        src/fx/algo/fxalgo_fir.c
        src/fx/algo/fxalgo_fir.h
)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

target_link_directories(dsptester PRIVATE .)
target_link_libraries(dsptester PRIVATE SDL3::SDL3 winmm fftw3f-3 Python3::Python)

target_include_directories(dsptester PRIVATE
        vendored
        .
        ASIOSDK
        ASIOSDK/common
        ASIOSDK/host
        ASIOSDK/host/pc
        src
        ${PYTHON_INCLUDE_DIRS}
        "C:/Users/Made/AppData/Local/Programs/Python/Python312/Lib/site-packages/numpy/_core/include/"
)