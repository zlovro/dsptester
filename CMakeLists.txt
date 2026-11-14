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
        src/gfx/st7920.c
        src/types.h
        src/gfx/ui_control.c
        src/gfx/ui_control.h
        src/audiofx/effect.c
        src/audiofx/effect.h
        src/audiofx/effect_distorsion.c
        src/audiofx/effect_distorsion.h
        src/audiofx/effect_eq.c
        src/audiofx/effect_eq.h
        src/wavstreamer.cpp
        src/wavstreamer.hpp
        src/dsp.c
        src/dsp.h
        src/gfx/ui_common.h
        src/audiofx/algo/fxalgo_xpass.c
        src/audiofx/algo/fxalgo_xpass.h
        src/audiofx/algo/fxalgo_fir.c
        src/audiofx/algo/fxalgo_fir.h
        src/audiofx/algo/fxalgo_eq.c
        src/audiofx/algo/fxalgo_eq.h
        src/pedalboard.c
        src/pedalboard.h
        src/audiofx/effect_noisegate.c
        src/audiofx/effect_noisegate.h
        src/audiofx/effect_volume.c
        src/audiofx/effect_volume.h
        src/audiofx/effect_chorus.c
        src/audiofx/effect_chorus.h
        src/audiofx/effect_delay.c
        src/audiofx/effect_delay.h
        src/audiofx/algo/fxalgo_noisegate.c
        src/audiofx/algo/fxalgo_noisegate.h
        src/audiofx/algo/fxalgo_volume.c
        src/audiofx/algo/fxalgo_volume.h
        src/input.c
        src/input.h
        src/audiofx/algo/fxalgo_ds.c
        src/audiofx/algo/fxalgo_ds.h
        src/iohelper.c
        src/iohelper.h
        src/gfx/gfx.c
        src/gfx/gfx.h
        src/gfx/ui_common.c
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