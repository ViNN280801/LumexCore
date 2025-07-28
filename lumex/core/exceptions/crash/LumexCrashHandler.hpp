#ifndef LUMEX_CRASH_HANDLER_HPP
#define LUMEX_CRASH_HANDLER_HPP

#include "lumex/LumexExport.hpp"
#include "lumex/core/utility/LumexUtility"

#include <string>

#if LUMEX_OS_WINDOWS
  #include <windows.h> // This library must be included before DbgHelp.h
                       // because DbgHelp.h uses types from Windows.h

  #include <DbgHelp.h>
  #include <tchar.h>
  #pragma comment(lib, "dbghelp.lib")
#else
  #include <csignal>
  #include <cstdlib>
  #include <fcntl.h>
  #include <sys/prctl.h>
  #include <sys/resource.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
#endif

namespace Lumex // NOLINT(modernize-concat-nested-namespaces)
{
  namespace Core
  {
    namespace Exceptions
    {
      namespace Crash
      {
        /**
         * @class LumexCrashHandler
         * @brief Handles application crashes by generating crash dumps (minidumps on Windows, core dumps on Unix).
         *
         * This class provides a cross-platform mechanism to capture crash information and notify the UI.
         * It supports both structured exception handling (SEH) on Windows and signal handling on Unix.
         */
        class LUMEX_API LumexCrashHandler
        {
        public:
          LumexCrashHandler(LumexCrashHandler const &)            = delete;
          LumexCrashHandler &operator=(LumexCrashHandler const &) = delete;
          LumexCrashHandler(LumexCrashHandler &&)                 = delete;
          LumexCrashHandler &operator=(LumexCrashHandler &&)      = delete;

          /**
           * @brief Returns the singleton instance of the crash handler.
           * @return Reference to the singleton instance.
           */
          static LumexCrashHandler &instance();

          /**
           * @brief Initializes the crash handler.
           *
           * Creates the crash dump directory and sets up platform-specific crash handlers.
           */
          void initialize(LUMEX_ATTRIBUTE_MAYBE_UNUSED std::string const &appName = s_defaultAppName);

#if LUMEX_OS_WINDOWS
          /**
           * @brief Windows-specific crash handler for unhandled exceptions.
           * @param pExInfo Pointer to exception information.
           * @return Execution disposition (always `EXCEPTION_EXECUTE_HANDLER`).
           */
          static LONG WINAPI _onWindowsCrashHandler(PEXCEPTION_POINTERS pExInfo);

          /**
           * @brief Wrapper for the Windows crash handler.
           * @param pExInfo Pointer to exception information.
           */
          void
          _handleSEHException(PEXCEPTION_POINTERS pExInfo)
          {
            _onWindowsCrashHandler(pExInfo);
          }
#endif

        private:
          LumexCrashHandler()  = default;
          ~LumexCrashHandler() = default;

#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251) // Suppress C4251 for STL members in DLL interface
#endif
          static std::string s_appName; ///< The name of the application.
#ifdef _WIN32
  #pragma warning(pop)
#endif
          static char const *s_defaultAppName;

          /**
           * @brief Generates a filename for the crash dump.
           * @param prefix Prefix for the dump filename.
           * @return Full path to the dump file.
           */
          static std::string _generateDumpFilename(std::string const &prefix);

          /**
           * @brief Notifies the UI and logs crash details.
           * @param errorMessage The error message to log and notify.
           */
          static void _notifyAndLog(std::string const &errorMessage);

#if LUMEX_OS_UNIX
          /// @brief Generates a core dump on Unix systems.
          static void _generateCoreDump();

          /**
           * @brief Signal handler for Unix systems.
           * @param signum The signal number.
           */
          static void _signalHandler(int signum);

          /// @brief Sets up signal handlers for Unix systems.
          static void _setupSignalHandlers();

          /// @brief Configures core dump settings on Unix systems.
          static void _setupCoreDumpSettings();
#endif
        };
      } // namespace Crash
    } // namespace Exceptions
  } // namespace Core
} // namespace Lumex

using LumexCrashHandler = Lumex::Core::Exceptions::Crash::LumexCrashHandler;

#endif // !LUMEX_CRASH_HANDLER_HPP
