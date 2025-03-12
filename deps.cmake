include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

function(find_packages)

    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(assimp CONFIG REQUIRED)
endfunction()

function(link_libs target)
    target_link_libraries(
            ${target}

            PRIVATE glfw
            PRIVATE glad::glad
            PRIVATE imgui::imgui
            PRIVATE spdlog::spdlog_header_only
            PRIVATE assimp::assimp
    )
endfunction()
