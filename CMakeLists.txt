cmake_minimum_required(VERSION 3.15)
project(prefixspan)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(prefixspan)
add_subdirectory(bindings/python)