cmake_minimum_required(VERSION 3.31)
project(dsptester)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# This assumes the SDL source is available in external/SDL
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

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

target_link_directories(dsptester PRIVATE run)
target_link_libraries(dsptester PRIVATE SDL3::SDL3 winmm fftw3f-3 Python3::Python)

set(NUMPY_DIR "${Python3_SITEARCH}/numpy/_core/include")

target_include_directories(dsptester PRIVATE
        external
        external/SDL/include
        .
        ASIOSDK
        ASIOSDK/common
        ASIOSDK/host
        ASIOSDK/host/pc
        src
        ${PYTHON_INCLUDE_DIRS}
        ${NUMPY_DIR}
)

set_target_properties(dsptester PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/run)