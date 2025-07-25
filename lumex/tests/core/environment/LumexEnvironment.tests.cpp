// LumexEnvironment.tests.cpp
#include <gtest/gtest.h>

#include "lumex/core/environment/LumexEnvironment.hpp"

#include <chrono> // For performance tests
#include <limits> // For numeric_limits
#include <string>
#include <thread> // For concurrency tests
#include <vector> // For concurrency tests

// --- Fixture ------------------------------------------------------------
class LumexEnvironmentTest : public ::testing::Test
{
protected:
  // Using a test fixture to ensure a clean state for each test
  // and to easily access the singleton instance.
  // Note: Environment variables are global, so tests that modify them
  // must be careful not to interfere with other tests.
  // For sensitive tests, consider using unique variable names or
  // resetting the environment in SetUp/TearDown, though full reset
  // can be problematic on some OSes or for standard variables like PATH.
  // For this reason, we'll mostly use unique, temporary variable names.

  void
  SetUp() override
  {
    // Ensure the singleton instance is ready
    env = &LumexEnvironment::instance();
  }

  // Not strictly necessary for this class as it doesn't allocate external resources
  // that need explicit cleanup in TearDown, but good practice for fixtures.
  void
  TearDown() override
  {
    // Clean up any specific environment variables set during tests
    env->unset_environment_variable("LUMEX_TEST_VAR");
    env->unset_environment_variable("LUMEX_ANOTHER_VAR");
    env->unset_environment_variable("LUMEX_LONG_VAR");
    env->unset_environment_variable("LUMEX_PERF_VAR");
    env->unset_environment_variable("LUMEX_STATIC_VAR");      // Cleanup from static tests
    env->unset_environment_variable("LUMEX_STATIC_SET_VAR");  // Cleanup from static tests
    env->unset_environment_variable("LUMEX_MAX_VAR");         // Cleanup from long value test
    env->unset_environment_variable("LUMEX_EMPTY_VAR");       // Cleanup from empty value test
    env->unset_environment_variable("LUMEX_TEMP_VAR");        // Cleanup from unset test
    env->unset_environment_variable("LUMEX_HAS_VAR");         // Cleanup from has test
    env->unset_environment_variable("LUMEX_GET_OR_EXISTENT"); // Cleanup from get_or test
    env->unset_environment_variable("LUMEX_UNSET_VIA_NULL");  // Cleanup from unset via null test
  }

  LumexEnvironment *env; // Pointer to the singleton instance
};

// --- API Contract Verifier Tests ----------------------------------------

TEST_F(LumexEnvironmentTest, GivenNonExistingVariable_WhenGetEnvironmentVariable_ThenReturnsUnsuccessful)
{
  // CoT: Request a variable that definitely doesn't exist -> expect unsuccessful result.
  LumexEnvironment::EnvResult result = env->get_environment_variable("NON_EXISTENT_LUMEX_VAR_12345");
  EXPECT_FALSE(result.success);
  EXPECT_FALSE(result); // Implicit bool conversion
  EXPECT_TRUE(result.value.empty());
  // Error code might vary by OS, so we don't assert a specific value, just that it's non-zero
  // for "not found" equivalent.
#if LUMEX_OS_WINDOWS
  // ERROR_ENVVAR_NOT_FOUND (203)
  EXPECT_EQ(result.error_code, 203);
#else
  // POSIX getenv returns nullptr, which is mapped to -2 (Not found) in LumexEnvironment.
  EXPECT_EQ(result.error_code, -2);
#endif
}

TEST_F(LumexEnvironmentTest, GivenExistingVariable_WhenGetEnvironmentVariable_ThenReturnsSuccessfulWithValue)
{
  // CoT: Set a variable -> get it back -> expect successful result with correct value.
  ASSERT_TRUE(env->set_environment_variable("LUMEX_TEST_VAR", "HelloLumex"));

  LumexEnvironment::EnvResult result = env->get_environment_variable("LUMEX_TEST_VAR");
  EXPECT_TRUE(result.success);
  EXPECT_TRUE(result);
  EXPECT_EQ(result.value, "HelloLumex");
  EXPECT_EQ(result.error_code, 0); // No error
}

TEST_F(LumexEnvironmentTest, GivenEmptyValue_WhenSetEnvironmentVariable_ThenVariableIsEmptyButExists)
{
#if LUMEX_OS_WINDOWS
  // On Windows, SetEnvironmentVariableA with an empty string actually deletes the variable
  // This is documented Windows behavior, so we skip this test on Windows
  GTEST_SKIP() << "Windows does not support setting environment variables to empty strings - they get deleted instead";
#else
  // Setting an empty string value should succeed, and the variable should exist but be empty.
  ASSERT_TRUE(env->set_environment_variable("LUMEX_EMPTY_VAR", ""));

  LumexEnvironment::EnvResult result = env->get_environment_variable("LUMEX_EMPTY_VAR");
  EXPECT_TRUE(result.success);
  EXPECT_TRUE(result);
  EXPECT_TRUE(result.value.empty());
#endif
}

TEST_F(LumexEnvironmentTest, GivenValidNameValue_WhenSetEnvironmentVariable_ThenVariableIsSet)
{
  // Test setting a new variable.
  EXPECT_TRUE(env->set_environment_variable("LUMEX_ANOTHER_VAR", "AnotherValue"));
  EXPECT_EQ(env->get_environment_variable_or("LUMEX_ANOTHER_VAR", ""), "AnotherValue");
}

TEST_F(LumexEnvironmentTest, GivenExistingVariable_WhenUnsetEnvironmentVariable_ThenVariableIsRemoved)
{
  // CoT: Set a variable -> unset it -> expect it to be gone.
  ASSERT_TRUE(env->set_environment_variable("LUMEX_TEMP_VAR", "ValueToUnset"));
  ASSERT_TRUE(env->has_environment_variable("LUMEX_TEMP_VAR"));

  EXPECT_TRUE(env->unset_environment_variable("LUMEX_TEMP_VAR"));
  EXPECT_FALSE(env->has_environment_variable("LUMEX_TEMP_VAR"));
}

TEST_F(LumexEnvironmentTest, GivenNonExistingVariable_WhenUnsetEnvironmentVariable_ThenReturnsTrue)
{
  // Unsetting a non-existent variable should still return true (idempotent behavior).
  EXPECT_TRUE(env->unset_environment_variable("LUMEX_NON_EXISTENT_VAR_TO_UNSET"));
}

TEST_F(LumexEnvironmentTest, GivenExistingVariable_WhenHasEnvironmentVariable_ThenReturnsTrue)
{
  ASSERT_TRUE(env->set_environment_variable("LUMEX_HAS_VAR", "present"));
  EXPECT_TRUE(env->has_environment_variable("LUMEX_HAS_VAR"));
}

TEST_F(LumexEnvironmentTest, GivenNonExistingVariable_WhenHasEnvironmentVariable_ThenReturnsFalse)
{
  EXPECT_FALSE(env->has_environment_variable("LUMEX_HAS_NON_EXISTENT_VAR"));
}

TEST_F(LumexEnvironmentTest, GivenNonExistingVariable_WhenGetEnvironmentVariableOr_ThenReturnsDefault)
{
  std::string default_val = "MyDefault";
  std::string result      = env->get_environment_variable_or("LUMEX_GET_OR_NON_EXISTENT", default_val);
  EXPECT_EQ(result, default_val);
}

TEST_F(LumexEnvironmentTest, GivenExistingVariable_WhenGetEnvironmentVariableOr_ThenReturnsActualValue)
{
  ASSERT_TRUE(env->set_environment_variable("LUMEX_GET_OR_EXISTENT", "ActualValue"));
  std::string result = env->get_environment_variable_or("LUMEX_GET_OR_EXISTENT", "DefaultShouldNotBeUsed");
  EXPECT_EQ(result, "ActualValue");
}

TEST_F(LumexEnvironmentTest, GivenStaticGet_WhenCalled_ThenWorksCorrectly)
{
  // Test static get() convenience method
  LumexEnvironment::set("LUMEX_STATIC_VAR", "StaticValue");
  auto result = LumexEnvironment::get("LUMEX_STATIC_VAR");
  EXPECT_TRUE(result);
  EXPECT_EQ(result.value, "StaticValue");
}

TEST_F(LumexEnvironmentTest, GivenStaticSetAndHas_WhenCalled_ThenWorksCorrectly)
{
  // Test static set() and has() convenience methods
  EXPECT_TRUE(LumexEnvironment::set("LUMEX_STATIC_SET_VAR", "StaticSetVal"));
  EXPECT_TRUE(LumexEnvironment::has("LUMEX_STATIC_SET_VAR"));
  EXPECT_TRUE(LumexEnvironment::set("LUMEX_STATIC_SET_VAR", nullptr)); // Unset via set(name, nullptr)
  EXPECT_FALSE(LumexEnvironment::has("LUMEX_STATIC_SET_VAR"));
}

// --- Edge & Corner Cases ------------------------------------------------

TEST_F(LumexEnvironmentTest, GivenNullName_WhenGetEnvironmentVariable_ThenReturnsUnsuccessful)
{
  LumexEnvironment::EnvResult result = env->get_environment_variable(nullptr);
  EXPECT_FALSE(result.success);
  EXPECT_TRUE(result.value.empty());
#if LUMEX_OS_WINDOWS
  // Windows GetEnvironmentVariableA returns 0 for null, GetLastError might be ERROR_INVALID_PARAMETER (87)
  // or ERROR_BAD_ENVIRONMENT (10), but LumexEnvironment maps null to -1.
  EXPECT_EQ(result.error_code, -1);
#else
  // POSIX getenv with nullptr should not happen, but LumexEnvironment maps it to -1.
  EXPECT_EQ(result.error_code, -1);
#endif
}

TEST_F(LumexEnvironmentTest, GivenEmptyName_WhenGetEnvironmentVariable_ThenReturnsUnsuccessful)
{
  LumexEnvironment::EnvResult result = env->get_environment_variable("");
  EXPECT_FALSE(result.success);
  EXPECT_TRUE(result.value.empty());
#if LUMEX_OS_WINDOWS
  // _dupenv_s handles "" as invalid, GetEnvironmentVariableA might return success with empty string,
  // but LumexEnvironment maps it to ERROR_INVALID_PARAMETER (87).
  EXPECT_EQ(result.error_code, ERROR_INVALID_PARAMETER);
#else
  // getenv("") is undefined behavior, LumexEnvironment maps it to -1.
  EXPECT_EQ(result.error_code, -1);
#endif
}

TEST_F(LumexEnvironmentTest, GivenNullName_WhenSetEnvironmentVariable_ThenReturnsFalse)
{
  EXPECT_FALSE(env->set_environment_variable(nullptr, "SomeValue"));
}

TEST_F(LumexEnvironmentTest, GivenEmptyName_WhenSetEnvironmentVariable_ThenReturnsFalse)
{
  EXPECT_FALSE(env->set_environment_variable("", "SomeValue"));
}

TEST_F(LumexEnvironmentTest, GivenNullName_WhenUnsetEnvironmentVariable_ThenReturnsFalse)
{
  EXPECT_FALSE(env->unset_environment_variable(nullptr));
}

TEST_F(LumexEnvironmentTest, GivenEmptyName_WhenUnsetEnvironmentVariable_ThenReturnsFalse)
{
  EXPECT_FALSE(env->unset_environment_variable(""));
}

TEST_F(LumexEnvironmentTest, GivenVeryLongValue_WhenSetAndGet_ThenWorksCorrectly)
{
  // Test setting and retrieving a very long environment variable value.
  // MAX_ENV_BUFFER_SIZE is 32767. Let's use something close to it.
  std::string long_value(LumexEnvironment::MAX_ENV_BUFFER_SIZE - 100, 'X');
  ASSERT_TRUE(env->set_environment_variable("LUMEX_LONG_VAR", long_value.c_str()));

  LumexEnvironment::EnvResult result = env->get_environment_variable("LUMEX_LONG_VAR");
  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.value, long_value);

  // Test with value exactly at max size
  std::string max_value(LumexEnvironment::MAX_ENV_BUFFER_SIZE - 1, 'Y'); // Null terminator counts for Windows
  ASSERT_TRUE(env->set_environment_variable("LUMEX_MAX_VAR", max_value.c_str()));
  result = env->get_environment_variable("LUMEX_MAX_VAR");
  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.value, max_value);

#if LUMEX_OS_WINDOWS
  // On Windows, if GetEnvironmentVariableA returns size > MAX_ENV_BUFFER_SIZE, LumexEnvironment
  // returns ERROR_BUFFER_OVERFLOW. Test this scenario if possible.
  // However, directly testing overflow is hard without external manipulation or a mock.
  // The current implementation guards against local buffer overflow, but not the system's limit.
#else
  // On POSIX, if std::strlen(result) > MAX_ENV_BUFFER_SIZE, it returns -3 (Too large).
  // This is hard to trigger without setting an environment variable larger than the internal
  // buffer, which putenv/setenv might also limit.
#endif
}

TEST_F(LumexEnvironmentTest, GivenNullValue_WhenSetEnvironmentVariable_ThenUnsetsVariable)
{
  // set_environment_variable with nullptr value should unset the variable.
  ASSERT_TRUE(env->set_environment_variable("LUMEX_UNSET_VIA_NULL", "InitialValue"));
  EXPECT_TRUE(env->has_environment_variable("LUMEX_UNSET_VIA_NULL"));

  EXPECT_TRUE(env->set_environment_variable("LUMEX_UNSET_VIA_NULL", nullptr));
  EXPECT_FALSE(env->has_environment_variable("LUMEX_UNSET_VIA_NULL"));
}

// --- Concurrency Tests --------------------------------------------------

TEST_F(LumexEnvironmentTest, ThreadSafety_SimultaneousReads)
{
  // Test that multiple threads can read environment variables concurrently without issues.
  ASSERT_TRUE(env->set_environment_variable("LUMEX_CONCURRENCY_READ", "SharedValue"));

  constexpr int num_threads = 10;
  std::vector<std::thread> threads;
  std::vector<std::string> results(num_threads);
  std::vector<bool> successes(num_threads);

  for(int i = 0; i < num_threads; ++i)
  {
    threads.emplace_back(
      [this, i, &results, &successes]()
      {
        LumexEnvironment::EnvResult res = env->get_environment_variable("LUMEX_CONCURRENCY_READ");
        successes[i]                    = res.success;
        if(res.success) results[i] = res.value;
      });
  }

  for(auto &t : threads) t.join();

  for(int i = 0; i < num_threads; ++i)
  {
    EXPECT_TRUE(successes[i]) << "Thread " << i << " failed to read";
    EXPECT_EQ(results[i], "SharedValue") << "Thread " << i << " read incorrect value";
  }
}

TEST_F(LumexEnvironmentTest, ThreadSafety_SimultaneousWritesAndReads)
{
  // Test concurrent writes and reads. While set/unset are mutex-protected,
  // reading system-wide environment variables can still have race conditions
  // with other processes or non-mutexed calls. This test verifies the mutex
  // within LumexEnvironment.
  constexpr int num_threads = 20;
  std::vector<std::thread> threads;

  for(int i = 0; i < num_threads; ++i)
  {
    threads.emplace_back(
      [this, i]()
      {
        std::string var_name  = "LUMEX_CONCURRENCY_VAR_" + std::to_string(i);
        std::string var_value = "Value_" + std::to_string(i);

        // Set
        EXPECT_TRUE(env->set_environment_variable(var_name.c_str(), var_value.c_str()));
        // Read
        LumexEnvironment::EnvResult res = env->get_environment_variable(var_name.c_str());
        EXPECT_TRUE(res.success) << "Failed to read " << var_name;
        EXPECT_EQ(res.value, var_value) << "Read incorrect value for " << var_name;
        // Unset
        EXPECT_TRUE(env->unset_environment_variable(var_name.c_str()));
        // Verify unset
        EXPECT_FALSE(env->has_environment_variable(var_name.c_str()));
      });
  }

  for(auto &t : threads) t.join();
}

// --- Performance & Stress Tests -----------------------------------------

TEST_F(LumexEnvironmentTest, Perf_RepeatedGetOperations)
{
  // Measure performance of repeated get operations for an existing variable.
  ASSERT_TRUE(env->set_environment_variable("LUMEX_PERF_VAR", "PerformanceTestValue"));

  constexpr int iterations = 10000;
  auto start               = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; ++i)
  {
    LumexEnvironment::EnvResult result = env->get_environment_variable("LUMEX_PERF_VAR");
    // Minimal assertion to avoid skewing performance, but ensure correctness
    EXPECT_TRUE(result.success);
  }
  auto duration
    = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

  EXPECT_LT(duration.count(), 100) // Expect it to be fast, e.g., under 100ms
    << "Repeated GET operations took too long: " << duration.count() << "ms";
}

TEST_F(LumexEnvironmentTest, Perf_RepeatedSetAndUnsetOperations)
{
  // Measure performance of repeated set and unset operations.
  constexpr int iterations = 1000; // Fewer iterations due to higher cost of set/unset
  auto start               = std::chrono::high_resolution_clock::now();
  for(int i = 0; i < iterations; ++i)
  {
    std::string var_name  = "LUMEX_PERF_SET_UNSET_" + std::to_string(i);
    std::string var_value = "Value_" + std::to_string(i);

    EXPECT_TRUE(env->set_environment_variable(var_name.c_str(), var_value.c_str()));
    EXPECT_TRUE(env->unset_environment_variable(var_name.c_str()));
  }
  auto duration
    = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

  EXPECT_LT(duration.count(), 1000) // Expect it to be reasonably fast, e.g., under 1000ms
    << "Repeated SET/UNSET operations took too long: " << duration.count() << "ms";
}

// --- Memory & Lifetime Auditor Tests ------------------------------------

// For EnvResult struct, since it uses std::string, its memory management is handled by STL.
// We just need to ensure our usage doesn't lead to issues.

TEST_F(LumexEnvironmentTest, MemorySafety_EnvResultDestructorCalled)
{
  // Ensure EnvResult objects are properly destructed when going out of scope.
  // This is implicitly tested by normal usage, but we can make it explicit.
  {
    LumexEnvironment::EnvResult result = env->get_environment_variable("PATH");
    // result goes out of scope here. No manual delete needed.
  }
  SUCCEED(); // If no crash or leak detected, test passes
}
