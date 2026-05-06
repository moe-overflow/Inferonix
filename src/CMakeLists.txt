file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.hpp")

add_executable(InferonixEngine ${SOURCE_FILES} EntryPoint.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/deps.cmake)
find_packages()
link_libs(InferonixEngine)
add_angelscript_addons(InferonixEngine)

target_include_directories(
        InferonixEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)


