cmake_minimum_required(VERSION 3.18)
project(WaterRippleEffect VERSION 0.0.1 LANGUAGES CXX)

# define compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra -Werror -Wconversion -Wstrict-overflow=2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wconversion
                        -Wundef -Wdeprecated
		                -Wweak-vtables -Wshadow)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W3 /WX /utf-8)
endif()

# setup include dir
set(WRE_INC_DIR include/wre)

# setup source dir
set(WRE_SRC_DIR src/)

# setup headers
set(HEADERS 
    ${WRE_INC_DIR}/wave_map.hpp
    ${WRE_INC_DIR}/water_ripple.h
)

# setup sources
set(SOURCES 
    ${WRE_SRC_DIR}/water_ripple.cpp
)

# create wre library
add_library(wre STATIC ${HEADERS} ${SOURCES})

# default include "include/" dir
target_include_directories(wre PUBLIC include/)

# remove "lib<name>" prefix
set_target_properties(wre PROPERTIES PREFIX "")

# define wre properties
set_target_properties(wre PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)