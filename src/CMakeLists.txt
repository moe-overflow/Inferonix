file(GLOB_RECURSE SOURCE_FILES "*.cpp" "*.hpp")

add_executable(InferonixEngine ${SOURCE_FILES} entry_point.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)
find_packages()
link_libs(InferonixEngine)
add_angelscript_addons(InferonixEngine)

target_include_directories(
        InferonixEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(InferonixEngine PRIVATE 
        SHADERS_PATH=\"${CMAKE_SOURCE_DIR}/shaders/\"
        RESOURCES_PATH=\"${CMAKE_SOURCE_DIR}/resources/\"
)

add_custom_command(TARGET InferonixEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:InferonixEngine>/assets
)


add_custom_command(TARGET InferonixEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:InferonixEngine>/shaders
)

