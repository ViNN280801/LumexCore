# lumex/tests/applied/hardware/CMakeLists.txt

set(TEST_EXECUTABLE_NAME LumexHardwareCapabilitiesTests)

add_executable(
    ${TEST_EXECUTABLE_NAME}
    LumexHardwareCapabilities.tests.cpp
)

set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

target_link_libraries(
    ${TEST_EXECUTABLE_NAME}
    PRIVATE
    GTest::gtest_main
    Lumex::hardware
)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
