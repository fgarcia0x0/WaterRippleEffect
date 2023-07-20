cmake_minimum_required(VERSION 3.20)

set(WRE_GUI_HEADERS 
    include/mainwindow.h
    include/water_effect.h)

set(WRE_GUI_SRCS 
    src/mainwindow.cpp
    src/main.cpp
    src/water_effect.cpp)

set(WRE_GUI_UI
    src/mainwindow.ui)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Stb REQUIRED)    
qt_standard_project_setup()

qt_add_executable(wre_gui ${WRE_GUI_HEADERS} ${WRE_GUI_SRCS} ${WRE_GUI_UI})
target_link_libraries(wre_gui PRIVATE wre Qt6::Widgets Qt6::Gui)

target_include_directories(wre_gui PUBLIC include/ ${Stb_INCLUDE_DIR})

# define wre properties
set_target_properties(wre_gui PROPERTIES
    EXPORT_COMPILE_COMMANDS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)