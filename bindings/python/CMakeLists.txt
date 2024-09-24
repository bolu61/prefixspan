cmake_minimum_required(VERSION 3.15...3.27)
project(prefixspan_python_ext)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_package(Python 3.11 REQUIRED COMPONENTS Interpreter Development.Module OPTIONAL_COMPONENTS Development.SABIModule)
execute_process(COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(prefixspan_python_ext STABLE_ABI NB_STATIC src/bindings.cxx)
target_link_libraries(prefixspan_python_ext PRIVATE prefixspan)
set_target_properties(prefixspan_python_ext PROPERTIES OUTPUT_NAME prefixspan)

install(TARGETS prefixspan_python_ext LIBRARY DESTINATION .)
