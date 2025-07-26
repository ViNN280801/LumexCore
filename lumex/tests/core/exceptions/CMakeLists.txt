# lumex/tests/core/exceptions/CMakeLists.txt

set(TEST_EXECUTABLE_NAME LumexExceptionsTests)

add_executable(
    ${TEST_EXECUTABLE_NAME}
    LumexException.tests.cpp
)

set_target_properties(${TEST_EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

target_link_libraries(
    ${TEST_EXECUTABLE_NAME}
    PRIVATE
    GTest::gtest_main
    Lumex::filesystem
    Lumex::exceptions
    Lumex::time
    Lumex::utility
)

if(WIN32)
    target_compile_options(${TEST_EXECUTABLE_NAME} PRIVATE

        # For Structured Exception Handling (SEH)
        /EHa

        # Generate program database (PDB) debug information for source files
        # /Zi is needed for full debug symbols with MSVC.
        /Zi
    )
    target_link_options(${TEST_EXECUTABLE_NAME} PRIVATE

        # Linker flag to generate debug information (PDB file)
        /DEBUG
    )
elseif(UNIX AND NOT APPLE)
    # Ensure debug symbols are generated and exported for the test executable
    target_compile_options(${TEST_EXECUTABLE_NAME} PRIVATE -g)
    target_link_options(${TEST_EXECUTABLE_NAME} PRIVATE -rdynamic) # needed for backtrace_symbols to resolve names
endif()

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
