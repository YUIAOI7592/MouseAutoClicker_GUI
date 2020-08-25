cmake_minimum_required (VERSION 2.6)
project (MouseAutoClicker)
set(EXECUTABLE_OUTPUT_PATH dist)
SET(CMAKE_CXX_FLAGS "-std=c++20 -lstdc++ -luser32 -mwindows")
include_directories(${MouseAutoClicker_SOURCE_DIR}/src)
add_executable(MouseAutoClicker
${MouseAutoClicker_SOURCE_DIR}/src/main.cpp)