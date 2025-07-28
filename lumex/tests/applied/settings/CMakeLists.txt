# lumex/tests/applied/settings/CMakeLists.txt

set(TEST_EXECUTABLE_NAME LumexSettingsTests)

add_executable(
    ${TEST_EXECUTABLE_NAME}
    LumexSettingsINI.tests.cpp
)

set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

target_link_libraries(
    ${TEST_EXECUTABLE_NAME}
    PRIVATE
    GTest::gtest_main
    Lumex::settings
)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
