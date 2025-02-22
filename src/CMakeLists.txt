file(
        GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.hpp"
)

add_library(
        ${CMAKE_PROJECT_NAME}
        STATIC
        ${SOURCE_FILES}
)


include(${CMAKE_SOURCE_DIR}/deps.cmake)
find_packages()
link_libs(${CMAKE_PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)


