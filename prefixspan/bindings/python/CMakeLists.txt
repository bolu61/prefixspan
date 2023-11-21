find_package(Python 3.11 REQUIRED COMPONENTS Interpreter Development.Module OPTIONAL_COMPONENTS Development.SABIModule)

# default to release build
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# detect the installed nanobind package
execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR
)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(prefixspan-python STABLE_ABI src/bindings.cxx)
target_include_directories(prefixspan-python PRIVATE "${PROJECT_SOURCE_DIR}/prefixspan/src")
target_link_libraries(prefixspan-python PRIVATE prefixspan)

install(TARGETS prefixspan-python LIBRARY DESTINATION prefixspan)
