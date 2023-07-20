cmake_minimum_required(VERSION 3.18)
project(WaterRippleEffect VERSION 0.0.1 LANGUAGES CXX)

# setup asan flags
set(WRE_ENABLE_ASAN OFF)
set(WRE_ASAN_FLAGS -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak -fsanitize=undefined)

# define compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra -Werror -Wstrict-overflow=2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Werror -Wstrict-overflow=2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W3 /WX /utf-8)
endif()

# setup include dir
set(WRE_INC_DIR include/wre)

# setup source dir
set(WRE_SRC_DIR src/)

# setup headers
set(WRE_HEADERS 
    ${WRE_INC_DIR}/wave_map.hpp
    ${WRE_INC_DIR}/water_ripple.h)

# setup sources
set(WRE_SOURCES 
    ${WRE_SRC_DIR}/water_ripple.cpp)

# create wre library
add_library(wre STATIC ${WRE_HEADERS} ${WRE_SOURCES})

# enable asan in debug build
if (${WRE_ENABLE_ASAN})
    target_compile_options(wre PUBLIC $<$<CONFIG:Debug>:${WRE_ASAN_FLAGS}>)
    target_link_options(wre PUBLIC $<$<CONFIG:Debug>:${WRE_ASAN_FLAGS}>)
endif()

# default include "include/" dir
target_include_directories(wre PUBLIC include/)

# remove "lib<name>" prefix
set_target_properties(wre PROPERTIES PREFIX "")

# define wre properties
set_target_properties(wre PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_subdirectory(gui/qt)
#add_subdirectory(test/)