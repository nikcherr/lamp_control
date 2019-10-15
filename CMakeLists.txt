cmake_minimum_required(VERSION 3.0)
project(lamp_control LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

set(HW_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/include)
set(HW_GUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gui)
include_directories(${HW_HEADER_DIR})

find_package(Qt5Widgets REQUIRED)
find_package(Threads)

qt5_wrap_cpp(lamp_control_SRC ${HW_HEADER_DIR}/mainwindow.h)
qt5_wrap_ui(lamp_control_UI ${HW_GUI_DIR}/mainwindow.ui)
add_executable(lamp_control source/main.cpp source/mainwindow.cpp 
		source/render_lantern.cpp source/tcp_client.cpp 
		source/tlv.cpp source/tlv_container.cpp source/tests.cpp source/parser.cpp source/include/tcp_client.h source/include/tlv_container.h
		${lamp_control_UI} ${lamp_control_SRC})

target_link_libraries (lamp_control ${CMAKE_THREAD_LIBS_INIT})
qt5_use_modules(lamp_control Widgets Core Gui Network)
