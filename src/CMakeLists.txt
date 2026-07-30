find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Angelscript CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Jolt CONFIG REQUIRED)
find_package(argparse CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)

add_library(inferonix_lib STATIC)

add_subdirectory(scene)
add_subdirectory(assets)
add_subdirectory(event)
add_subdirectory(input)
add_subdirectory(physics)
add_subdirectory(renderer)
add_subdirectory(script)
add_subdirectory(ui)
add_subdirectory(util)
add_subdirectory(window)

target_sources(inferonix_lib PRIVATE inferonix_engine.hpp inferonix_engine.cpp inferonix_config.hpp)

add_executable(inferonix entry_point.cpp)


target_include_directories(
        inferonix_lib PUBLIC
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(inferonix_lib PUBLIC
        glfw
        glad::glad
        imgui::imgui
        spdlog::spdlog_header_only
        assimp::assimp
        EnTT::EnTT
        Angelscript::angelscript
        Boost::headers
        nlohmann_json::nlohmann_json
        Jolt::Jolt
        argparse::argparse
        imguizmo::imguizmo
)

target_link_libraries(inferonix PUBLIC inferonix_lib)

target_compile_definitions(inferonix_lib PRIVATE
        SHADERS_PATH=\"${CMAKE_SOURCE_DIR}/shaders/\"
        RESOURCES_PATH=\"${CMAKE_SOURCE_DIR}/resources/\"
)

add_custom_command(TARGET inferonix POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:inferonix>/assets
)


add_custom_command(TARGET inferonix POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:inferonix>/shaders
)

# angelscript addons
get_target_property(ANGELSCRIPT_INCLUDE_PATH Angelscript::angelscript INTERFACE_INCLUDE_DIRECTORIES)

if(NOT ANGELSCRIPT_INCLUDE_PATH)
    message(WARNING "Could not find angelscript include path")
    return()
endif()

target_sources(inferonix PRIVATE
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptstdstring/scriptstdstring.cpp"
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptmath/scriptmath.cpp"
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptbuilder/scriptbuilder.cpp"
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptarray/scriptarray.cpp"
)
target_include_directories(inferonix PRIVATE "${ANGELSCRIPT_INCLUDE_PATH}")

target_precompile_headers(inferonix_lib PRIVATE
        inferonix_pch.hpp
)

