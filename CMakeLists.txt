cmake_minimum_required(VERSION 3.25)

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


include(${CMAKE_SOURCE_DIR}/deps.cmake)
fetch_deps()

add_subdirectory(src)
add_subdirectory(vendor)

