include(GoogleTest)
find_package(GTest CONFIG REQUIRED)

add_executable(unit_tests
        asset_registry_tests.cpp

)

target_link_libraries(unit_tests inferonix_lib GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

