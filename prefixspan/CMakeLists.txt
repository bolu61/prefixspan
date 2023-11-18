add_library(prefixspan INTERFACE src/prefixspan.hxx)
target_include_directories(prefixspan INTERFACE $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

add_subdirectory(bindings/python)
