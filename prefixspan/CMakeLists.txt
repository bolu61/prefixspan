cmake_minimum_required(VERSION 3.15)
project(prefixspan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_library(prefixspan INTERFACE src/prefixspan.hxx src/trie.hxx)
target_include_directories(prefixspan INTERFACE src)

add_executable(test_prefixspan tests/test_prefixspan.cxx)
target_link_libraries(test_prefixspan PRIVATE prefixspan)
add_test(NAME test_prefixspan COMMAND test_prefixspan)
