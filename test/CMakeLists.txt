include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

add_executable(unit_tests
        asset_registry_tests.cpp
        camera_tests.cpp
        command_registry_tests.cpp
        random_tests.cpp
        transform_tests.cpp
        uuid_tests.cpp
)

target_link_libraries(unit_tests inferonix_lib GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_compile_definitions(unit_tests PRIVATE
        TEST_ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
)

