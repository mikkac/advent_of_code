cmake_minimum_required(VERSION 3.16)
project(aoc22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(d01 d01/d01.cpp)
target_link_libraries(d01 ${CONAN_LIBS})

add_executable(d02 d02/d02.cpp)
target_link_libraries(d02 ${CONAN_LIBS})

add_executable(d03 d03/d03.cpp)
target_link_libraries(d03 ${CONAN_LIBS})

add_executable(d04 d04/d04.cpp)
target_link_libraries(d04 ${CONAN_LIBS})

