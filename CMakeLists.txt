cmake_minimum_required(VERSION 3.15)
project(prefixspan)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_library(prefixspan INTERFACE)
target_include_directories(prefixspan INTERFACE src)

if (BUILD_TESTING)
    find_package(OpenMP REQUIRED)
    find_package(Catch2 3 REQUIRED)
    add_executable(test_prefixspan tests/test_prefixspan.cxx)
    target_link_libraries(test_prefixspan PRIVATE prefixspan Catch2::Catch2WithMain OpenMP::OpenMP_CXX)
    add_test(NAME test_prefixspan COMMAND test_prefixspan)
endif()
