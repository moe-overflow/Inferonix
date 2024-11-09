include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)


function(fetch_deps)

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
    CPMAddPackage("gh:g-truc/glm#0.9.9.8")



endfunction(fetch_deps)
