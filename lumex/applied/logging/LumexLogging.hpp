#ifndef LUMEX_LOGGING_HPP
#define LUMEX_LOGGING_HPP

#include <cstdint>
#include <mutex>
#include <string>

#include "lumex/LumexExport.hpp"
#include "lumex/core/filesystem/LumexFilesystem" // For `Path`
#include "lumex/core/string/LumexString"         // For `stringify`

namespace Lumex
{
  namespace Applied
  {
    namespace Logging
    {
      /// @brief Defines the severity levels for logging.
      enum class LumexLogLevel : std::uint8_t
      {
        Debug    = 10,
        Info     = 20,
        Success  = 25,
        Warning  = 30,
        Error    = 40,
        Critical = 50
      };

      /**
       * @class Logger
       * @brief Thread-safe singleton logger supporting multiple modules.
       *
       * Logger provides methods for logging messages at different severity levels,
       * includes a custom SUCCESS level, and outputs colored messages to the console
       * using ANSI escape codes.
       */
      class LUMEX_API LumexLogging
      {
      public:
        /// @brief Log a DEBUG-level message.
        template <typename... Args>
        static void
        debug(char const *moduleName, Args &&...args)
        {
          _log(LumexLogLevel::Debug, moduleName, stringify(std::forward<Args>(args)...));
        }

        /// @brief Log an INFO-level message.
        template <typename... Args>
        static void
        info(char const *moduleName, Args &&...args)
        {
          _log(LumexLogLevel::Info, moduleName, stringify(std::forward<Args>(args)...));
        }

        /// @brief Log a SUCCESS-level message (custom level).
        template <typename... Args>
        static void
        success(char const *moduleName, Args &&...args)
        {
          _log(LumexLogLevel::Success, moduleName, stringify(std::forward<Args>(args)...));
        }

        /// @brief Log a WARNING-level message.
        template <typename... Args>
        static void
        warning(char const *moduleName, Args &&...args)
        {
          _log(LumexLogLevel::Warning, moduleName, stringify(std::forward<Args>(args)...));
        }

        /// @brief Log an ERROR-level message.
        template <typename... Args>
        static void
        error(char const *moduleName, Args &&...args)
        {
          _log(LumexLogLevel::Error, moduleName, stringify(std::forward<Args>(args)...));
        }

        /// @brief Log a CRITICAL-level message.
        template <typename... Args>
        static void
        critical(char const *moduleName, Args &&...args)
        {
          _log(LumexLogLevel::Critical, moduleName, stringify(std::forward<Args>(args)...));
        }

        /**
         * @brief Write a log to a file in the logs/ directory
         * @param filename The name of the log file
         * @param level The log level
         * @param moduleName The name of the module
         * @param msg The message
         * @param appendTimestamp Whether to add a timestamp to the file name
         * @return true in case of success, false otherwise
         */
        static bool toFile(char const *filename, LumexLogLevel level, char const *moduleName, char const *msg,
                           bool appendTimestamp = true);

        /**
         * @brief Get the path to the logs directory
         * @return The path to the logs directory
         */
        static Lumex::Path getLogsDirectory();

        /**
         * @brief Set the name of the application
         * @param appName The name of the application
         */
        static void setAppName(std::string const &appName);

      private:
        static constexpr char const *KDEFAULT_LOG_FILE_NAME = "log"; ///< Default log file name.
        static constexpr short KLOG_WIDTH                   = 8;     ///< The width of the log level.

#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251) // Suppress C4251 for STL members in DLL interface
#endif
        static std::string s_appName;         ///< The name of the application, by default it would be empty.
        static std::mutex s_mutex;            ///< Mutex for thread-safe logging.
        static Lumex::Path s_logsDirectory;   ///< The directory for storing logs.
        static std::string s_launchTimestamp; ///< The timestamp when the application was launched.
#ifdef _WIN32
  #pragma warning(pop)
#endif

        /// @brief Core logging routine: formats and outputs everything to the
        /// console.
        static void _log(LumexLogLevel level, std::string const &moduleName, std::string const &msg);

        /// @brief Converts the level to a string ("DEBUG", "INFO", …).
        static char const *_levelToString(LumexLogLevel level) noexcept;

        /// @brief ANSI‑color for the given level.
        static char const *_levelToColor(LumexLogLevel level) noexcept;
      };
    } // namespace Applied
  } // namespace Logging
} // namespace Lumex

using LumexLogging = Lumex::Applied::Logging::LumexLogging;

#endif // !LUMEX_LOGGING_HPP
