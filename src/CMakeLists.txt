include(${CMAKE_SOURCE_DIR}/cmake/cmake_package_manager.cmake)

set(
        SOURCE_FILES
        main.cpp
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


