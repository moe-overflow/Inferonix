include(${CMAKE_SOURCE_DIR}/cmake/cmake_package_manager.cmake)

set(
        SOURCE_FILES

        Interface/application.cpp
        Interface/application.hpp
        Interface/callback_functions.cpp
        Interface/callback_functions.hpp
        Interface/main.hpp
        Interface/window.cpp
        Interface/window.hpp

        Renderer/buffer.hpp
        Renderer/index_buffer.cpp
        Renderer/renderer.cpp
        Renderer/renderer.hpp
        Renderer/shader.cpp
        Renderer/shader.hpp
        Renderer/shader_program.cpp
        Renderer/shader_program.hpp
        Renderer/vertex_array.hpp
        Renderer/vertex_array.cpp
        Renderer/vertex_buffer.cpp
)

add_library(
        ${CMAKE_PROJECT_NAME}
        STATIC
        ${SOURCE_FILES}
)

CPMAddPackage(
        NAME SPDLOG
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.12.0
        OPTIONS
            "SPDLOG_BUILD_EXAMPLE OFF"
            "SPDLOG_BUILD_TESTS OFF"
)

CPMAddPackage(
        NAME GLFW
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.3.9
        OPTIONS
            "GLFW_BUILD_TESTS OFF"
            "GLFW_BUILD_EXAMPLES OFF"
            "GLFW_BULID_DOCS OFF"
)

##

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC glfw
        PUBLIC glad
        PUBLIC spdlog::spdlog
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)


