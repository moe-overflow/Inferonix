cmake_minimum_required(VERSION 3.25)

include(${CMAKE_SOURCE_DIR}/cmake/vcpkg_toolchain.cmake)
set_vcpkg_toolchain()

project(
        Inferonix
        VERSION 0.1
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(cmake/PathDefinitions.cmake)

add_subdirectory(src/Engine)
add_subdirectory(src/Workbench)

