cmake_minimum_required(VERSION 3.24)
project(aoc22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(d01 d01.cpp)
target_link_libraries(d01 ${CONAN_LIBS})


