#ifndef LUMEX_ENVIRONMENT_HPP
#define LUMEX_ENVIRONMENT_HPP

#include "lumex/core/utility/LumexUtility"

#include <cstring> // std::strlen, std::strcpy
#include <memory>  // std::unique_ptr, std::addressof
#include <mutex>   // std::mutex, std::lock_guard
#include <string>  // std::string

#if LUMEX_OS_WINDOWS
  #include <cstdlib> // _dupenv_s, free
  #include <utility>
  #include <windows.h> // GetEnvironmentVariableA/W
#endif

#include "lumex/LumexExport.hpp"

namespace Lumex
{
  namespace Core
  {
    namespace Environment
    {
      /**
       * @brief Provides a cross-platform, and thread-safe way to manage
       *        environment variables.
       * @details This class implements the Singleton pattern to ensure a single,
       *          globally accessible instance for environment variable operations.
       *          It leverages the Strategy pattern internally to handle OS-specific
       *          details (Windows, Linux, macOS) while presenting a unified API.
       *          All operations are designed to be exception-safe, returning a
       *          dedicated result structure (EnvResult) with status and error codes
       *          instead of throwing exceptions. Resource management is handled using
       *          RAII principles, particularly for dynamically allocated buffers.
       *          This class is non-copyable and non-movable to maintain the
       *          integrity of the singleton instance.
       *
       * @note This class adheres to C++11 standard and uses appropriate
       * platform-specific APIs for optimal performance and security (e.g., _dupenv_s
       * on Windows).
       *
       * @example
       * // Basic usage:
       * LumexEnvironment::EnvResult path_result = LumexEnvironment::get("PATH");
       * if (path_result.success) {
       *     std::cout << "PATH: " << path_result.value << std::endl;
       * } else {
       *     std::cerr << "Failed to get PATH. Error code: " << path_result.error_code
       * << std::endl;
       * }
       *
       * // Getting with a default fallback value:
       * std::string home_dir = LumexEnvironment::get_or("HOME", "/default/home");
       * std::cout << "Home directory: " << home_dir << std::endl;
       *
       * // Setting an environment variable:
       * if (LumexEnvironment::set("MY_APP_VAR", "my_value")) {
       *     std::cout << "MY_APP_VAR set successfully." << std::endl;
       * } else {
       *     std::cerr << "Failed to set MY_APP_VAR." << std::endl;
       * }
       *
       * // Checking if an environment variable exists:
       * if (LumexEnvironment::has("TEMP")) {
       *     std::cout << "TEMP variable exists." << std::endl;
       * }
       */
      class LUMEX_API LumexEnvironment
      {
      public:
        // Type aliases for clarity and maintainability
        using size_type   = std::size_t;
        using string_type = std::string;

        // Maximum buffer size for environment variables (reasonable limit)
        static size_type const MAX_ENV_BUFFER_SIZE = 32767; // Windows MAX_PATH * 16

        /**
         * @brief Represents the result of an environment variable operation.
         * @details This structure encapsulates the variable's value, a success flag,
         *          and an error code (if applicable). It implements the Null Object
         *          pattern, providing a safe default state and methods to retrieve
         *          values with fallbacks, preventing null pointer dereferences or
         *          exceptions.
         *
         * @example
         * LumexEnvironment::EnvResult result =
         * LumexEnvironment::get("NON_EXISTENT_VAR"); if (!result) { // Implicit
         * conversion to bool for convenience std::cerr << "Variable not found. Error
         * code: " << result.error_code << std::endl; std::string fallback =
         * result.get_value_or("DEFAULT_FALLBACK"); std::cout << "Using fallback
         * value: " << fallback << std::endl;
         * }
         */
        struct EnvResult {
          string_type value; ///< The value of the environment variable. Empty if not
                             ///< found or error.
          bool success;      ///< True if the operation was successful, false otherwise.
          int error_code;    ///< System-specific error code if `success` is false.

          /**
           * @brief Constructs an EnvResult in a default (unsuccessful) state.
           */
          EnvResult() : success(false), error_code(0) {}

          /**
           * @brief Constructs a successful EnvResult with a given value.
           * @param val The successful string value of the environment variable.
           */
          explicit EnvResult(string_type val) : value(std::move(val)), success(true), error_code(0) {}

          /**
           * @brief Constructs an unsuccessful EnvResult with an error code and an
           * optional fallback value.
           * @param err_code The error code indicating the reason for failure.
           * @param fallback An optional string to use as a fallback value if the
           * operation failed.
           */
          EnvResult(int err_code, string_type fallback = string_type())
              : value(std::move(fallback)), success(false), error_code(err_code)
          {}

          /**
           * @brief Retrieves the variable's value or a specified fallback value if
           * the operation failed.
           * @param fallback The string to return if the operation was not successful.
           * @return The actual value if successful, otherwise the provided fallback
           * value.
           */
          string_type const &
          get_value_or(string_type const &fallback) const
          {
            return success ? value : fallback;
          }

          /**
           * @brief Allows implicit conversion to bool to easily check the success
           * status.
           * @return True if the operation was successful, false otherwise.
           */
          operator bool() const { return success; }
        };

      private:
        // Strategy pattern: OS-specific implementation interface
        class LUMEX_API EnvironmentStrategy
        {
        public:
          virtual ~EnvironmentStrategy()                                       = default;
          virtual EnvResult get_variable(char const *name) const               = 0;
          virtual bool set_variable(char const *name, char const *value) const = 0;
          virtual bool unset_variable(char const *name) const                  = 0;
        };

#if LUMEX_OS_WINDOWS
        // Windows-specific implementation using Strategy pattern
        class LUMEX_API WindowsEnvironmentStrategy : public EnvironmentStrategy
        {
        public:
          EnvResult get_variable(char const *name) const override;

          bool set_variable(char const *name, char const *value) const override;

          bool unset_variable(char const *name) const override;
        };

#else
        // POSIX-compliant implementation (Linux, macOS, Unix)
        class LUMEX_API PosixEnvironmentStrategy : public EnvironmentStrategy
        {
        public:
          EnvResult get_variable(char const *name) const override;

          bool set_variable(char const *name, char const *value) const override;

          bool unset_variable(char const *name) const override;
        };
#endif

#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251)
#endif
        mutable std::mutex m_mutex; ///< Thread-safety mutex (mutable to allow const methods to lock)
        std::unique_ptr<EnvironmentStrategy> m_strategy; ///< Strategy instance (created once, used throughout lifetime)
#ifdef _WIN32
  #pragma warning(pop)
#endif

        // Private constructor for Singleton-like behavior (optional)
        LumexEnvironment() : m_strategy(create_strategy()) {}

        // Factory method for creating appropriate strategy
        static std::unique_ptr<EnvironmentStrategy>
        create_strategy()
        {
#if LUMEX_OS_WINDOWS
          return std::unique_ptr<EnvironmentStrategy>(new WindowsEnvironmentStrategy());
#else
          return std::unique_ptr<EnvironmentStrategy>(new PosixEnvironmentStrategy());
#endif
        }

      public:
        /**
         * @brief Provides the single, globally accessible instance of
         * LumexEnvironment.
         * @details This method implements the thread-safe Singleton pattern (using
         *          C++11 static initialization magic) to ensure only one instance
         *          of the environment manager exists throughout the application's
         * lifetime.
         * @return A reference to the singleton LumexEnvironment instance.
         *
         * @example
         * LumexEnvironment& env_instance = LumexEnvironment::instance();
         * // Now use env_instance to call non-static methods if preferred
         * LumexEnvironment::EnvResult path_res =
         * env_instance.get_environment_variable("PATH");
         */
        static LumexEnvironment &instance();

        /**
         * @brief Deleted copy constructor.
         * @details Ensures that LumexEnvironment objects cannot be copied,
         *          maintaining the integrity of the singleton pattern.
         */
        LumexEnvironment(LumexEnvironment const &) = delete;

        /**
         * @brief Deleted copy assignment operator.
         * @details Ensures that LumexEnvironment objects cannot be assigned,
         *          maintaining the integrity of the singleton pattern.
         */
        LumexEnvironment &operator=(LumexEnvironment const &) = delete;

        /**
         * @brief Default destructor.
         * @details Explicitly defined to ensure proper resource management and
         *          adherence to the Rule of Five.
         */
        ~LumexEnvironment() = default;

        /**
         * @brief Deleted move constructor.
         * @details Ensures that LumexEnvironment objects cannot be moved,
         *          maintaining the integrity of the singleton pattern.
         */
        LumexEnvironment(LumexEnvironment &&) = delete;

        /**
         * @brief Deleted move assignment operator.
         * @details Ensures that LumexEnvironment objects cannot be move-assigned,
         *          maintaining the integrity of the singleton pattern.
         */
        LumexEnvironment &operator=(LumexEnvironment &&) = delete;

        /**
         * @brief Retrieves the value of a specified environment variable in a
         * thread-safe manner.
         * @details This method provides a cross-platform way to get environment
         * variable values. It handles different OS-specific APIs internally and
         * returns an EnvResult structure, which indicates success or failure and
         * contains the value or an error code, without throwing exceptions.
         * @param name The null-terminated C-string name of the environment variable
         * to retrieve. Must not be nullptr or an empty string.
         * @return An EnvResult object containing the variable's value and operation
         * status. If `name` is invalid, returns an unsuccessful EnvResult with an
         * error code.
         *
         * @example
         * LumexEnvironment::EnvResult user_result =
         * LumexEnvironment::instance().get_environment_variable("USER"); if
         * (user_result.success) { std::cout << "Current user: " << user_result.value
         * << std::endl; } else { std::cerr << "Could not get USER variable. Error: "
         * << user_result.error_code << std::endl;
         * }
         */
        EnvResult get_environment_variable(char const *name) const;

        /**
         * @brief Convenience overload for `get_environment_variable` using
         * `std::string`.
         * @param name The `std::string` name of the environment variable to retrieve.
         * @return An EnvResult object containing the variable's value and operation
         * status.
         */
        EnvResult get_environment_variable(string_type const &name) const;

        /**
         * @brief Sets or updates the value of a specified environment variable in a
         * thread-safe manner.
         * @details This method allows setting new environment variables or modifying
         * existing ones. On POSIX systems, it attempts to use `setenv` if available
         * (C++11 + POSIX.1-2001), falling back to `putenv` for broader compatibility.
         * On Windows, it uses `SetEnvironmentVariableA`.
         * @param name The null-terminated C-string name of the environment variable
         * to set. Must not be nullptr or an empty string.
         * @param value The null-terminated C-string value to assign to the variable.
         *              If `nullptr`, the variable will be unset.
         * @return True if the variable was successfully set or unset, false
         * otherwise.
         *
         * @example
         * if (LumexEnvironment::instance().set_environment_variable("LOG_LEVEL",
         * "DEBUG")) { std::cout << "LOG_LEVEL set to DEBUG." << std::endl; } else {
         *     std::cerr << "Failed to set LOG_LEVEL." << std::endl;
         * }
         *
         * // Unsetting a variable:
         * if (LumexEnvironment::instance().set_environment_variable("OLD_VAR",
         * nullptr)) { std::cout << "OLD_VAR unset successfully." << std::endl;
         * }
         */
        bool set_environment_variable(char const *name, char const *value) const;

        /**
         * @brief Convenience overload for `set_environment_variable` using
         * `std::string`.
         * @param name The `std::string` name of the environment variable to set.
         * @param value The `std::string` value to assign to the variable.
         * @return True if the variable was successfully set, false otherwise.
         */
        bool set_environment_variable(string_type const &name, string_type const &value) const;

        /**
         * @brief Unsets (removes) a specified environment variable in a thread-safe
         * manner.
         * @details This method removes an environment variable. On Windows, it sets
         * the variable's value to nullptr. On POSIX systems, it uses `unsetenv` if
         * available, or a fallback of setting the variable to an empty string.
         * @param name The null-terminated C-string name of the environment variable
         * to unset. Must not be nullptr or an empty string.
         * @return True if the variable was successfully unset, false otherwise.
         *
         * @example
         * if (LumexEnvironment::instance().unset_environment_variable("TEMP_VAR")) {
         *     std::cout << "TEMP_VAR unset successfully." << std::endl;
         * } else {
         *     std::cerr << "Failed to unset TEMP_VAR." << std::endl;
         * }
         */
        bool unset_environment_variable(char const *name) const;

        /**
         * @brief Retrieves the value of an environment variable, providing a default
         * fallback.
         * @details This is a convenience method that returns the variable's value if
         * found, or a specified `default_value` if the variable does not exist or
         *          could not be retrieved. It simplifies usage by avoiding manual
         *          `EnvResult` checking for simple retrieval scenarios.
         * @param name The null-terminated C-string name of the environment variable.
         * @param default_value The `std::string` value to return if the variable is
         * not found.
         * @return The value of the environment variable, or `default_value` if not
         * found.
         *
         * @example
         * std::string editor =
         * LumexEnvironment::instance().get_environment_variable_or("EDITOR", "vim");
         * std::cout << "Preferred editor: " << editor << std::endl;
         */
        string_type get_environment_variable_or(char const *name, string_type const &default_value) const;

        /**
         * @brief Checks if a specified environment variable exists.
         * @details This is a convenience method that quickly determines the presence
         *          of an environment variable without retrieving its value.
         * @param name The null-terminated C-string name of the environment variable
         * to check.
         * @return True if the environment variable exists and was successfully
         * retrieved (even if empty), false otherwise.
         *
         * @example
         * if (LumexEnvironment::instance().has_environment_variable("PROGRAMFILES"))
         * { std::cout << "PROGRAMFILES variable is present." << std::endl;
         * }
         */
        bool has_environment_variable(char const *name) const;

        // Static convenience methods for easier usage
        /**
         * @brief Static convenience method to retrieve an environment variable.
         * @details This is a shortcut for
         * `LumexEnvironment::instance().get_environment_variable(name)`.
         * @param name The null-terminated C-string name of the environment variable.
         * @return An EnvResult object containing the variable's value and operation
         * status.
         *
         * @example
         * auto locale_result = LumexEnvironment::get("LANG");
         * if (locale_result) {
         *     std::cout << "System language: " << locale_result.value << std::endl;
         * }
         */
        static EnvResult get(char const *name);

        /**
         * @brief Static convenience method to retrieve an environment variable with a
         * default fallback.
         * @details This is a shortcut for
         * `LumexEnvironment::instance().get_environment_variable_or(name,
         * default_value)`.
         * @param name The null-terminated C-string name of the environment variable.
         * @param default_value The default value to return if the variable is not
         * found.
         * @return The value of the environment variable, or `default_value` if not
         * found.
         *
         * @example
         * std::string temp_path = LumexEnvironment::get_or("TMP", "/tmp");
         * std::cout << "Temporary path: " << temp_path << std::endl;
         */
        static string_type get_or(char const *name, string_type const &default_value);

        /**
         * @brief Static convenience method to set an environment variable.
         * @details This is a shortcut for
         * `LumexEnvironment::instance().set_environment_variable(name, value)`.
         * @param name The null-terminated C-string name of the environment variable.
         * @param value The null-terminated C-string value to assign (nullptr to
         * unset).
         * @return True if successful, false otherwise.
         *
         * @example
         * if (LumexEnvironment::set("APP_MODE", "PRODUCTION")) {
         *     std::cout << "App mode set to PRODUCTION." << std::endl;
         * }
         */
        static bool set(char const *name, char const *value);

        /**
         * @brief Static convenience method to check if an environment variable
         * exists.
         * @details This is a shortcut for
         * `LumexEnvironment::instance().has_environment_variable(name)`.
         * @param name The null-terminated C-string name of the environment variable.
         * @return True if the variable exists, false otherwise.
         *
         * @example
         * if (LumexEnvironment::has("HOME")) {
         *     std::cout << "HOME variable is defined." << std::endl;
         * }
         */
        static bool has(char const *name);
      };
    } // namespace Environment
  } // namespace Core
} // namespace Lumex

using LumexEnvironment = Lumex::Core::Environment::LumexEnvironment;

#endif // !LUMEX_ENVIRONMENT_HPP
