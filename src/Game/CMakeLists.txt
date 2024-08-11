
add_executable(game
        "game.cpp"
)

set_target_properties(game
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_SOURCE_DIR}/bin/game"
)

target_include_directories(
        game
        PRIVATE ${CMAKE_SOURCE_DIR}/shaders
)

target_link_libraries(game
        PUBLIC engine
)

add_custom_command(TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:game>/shaders

)


