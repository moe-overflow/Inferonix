include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

function(find_packages)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(imgui CONFIG REQUIRED)
    find_package(assimp CONFIG REQUIRED)
    find_package(EnTT CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(Angelscript CONFIG REQUIRED)
    find_package(Boost REQUIRED)
endfunction()

function(link_libs target)
    target_link_libraries(
            ${target}
            PRIVATE glfw
            PRIVATE glad::glad
            PRIVATE imgui::imgui
            PRIVATE spdlog::spdlog_header_only
            PRIVATE assimp::assimp
            PRIVATE EnTT::EnTT
            PRIVATE Angelscript::angelscript
            PRIVATE Boost::headers
    )
endfunction()

function(add_angelscript_addons target)
    get_target_property(ANGELSCRIPT_INCLUDE_PATH Angelscript::angelscript INTERFACE_INCLUDE_DIRECTORIES)
    
    if(NOT ANGELSCRIPT_INCLUDE_PATH)
        message(WARNING "Could not find angelscript include path")
        return()
    endif()

    target_sources(${target} PRIVATE
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptstdstring/scriptstdstring.cpp"
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptmath/scriptmath.cpp"
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptbuilder/scriptbuilder.cpp"
        "${ANGELSCRIPT_INCLUDE_PATH}/angelscript/scriptarray/scriptarray.cpp"
    )
    target_include_directories(${target} PRIVATE "${ANGELSCRIPT_INCLUDE_PATH}")
endfunction()

