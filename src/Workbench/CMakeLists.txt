file(
        GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.hpp"
)

add_executable(
        InferonixWorkbench
        ${SOURCE_FILES}
)

target_link_libraries(InferonixWorkbench PRIVATE InferonixEngine)

include(${CMAKE_SOURCE_DIR}/deps.cmake)
find_packages()
link_libs(InferonixWorkbench)

target_include_directories(
        InferonixWorkbench
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
