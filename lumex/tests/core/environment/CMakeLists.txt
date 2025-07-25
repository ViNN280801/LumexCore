# lumex/tests/core/environment/CMakeLists.txt

set(TEST_EXECUTABLE_NAME LumexEnvironmentTests)

add_executable(
    ${TEST_EXECUTABLE_NAME}
    LumexEnvironment.tests.cpp
)

set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

target_link_libraries(
    ${TEST_EXECUTABLE_NAME}
    PRIVATE
    GTest::gtest_main
    Lumex::environment
)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
