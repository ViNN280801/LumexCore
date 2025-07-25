#define LUMEX_IMPLEMENTATION
#include "LumexLogging.hpp"

#include "lumex/core/environment/LumexEnvironment"
#include "lumex/core/filesystem/LumexFilesystem"
#include "lumex/core/time/LumexTime"
#include "lumex/core/utility/LumexUtility"

#if LUMEX_OS_WINDOWS
  #include <windows.h>
#else
  #include <unistd.h>
#endif

#include <fstream>
#include <iomanip>
#include <iostream>

LUMEX_PUBLIC_API std::mutex LumexLogging::s_mutex;
LUMEX_PUBLIC_API Lumex::Path LumexLogging::s_logsDirectory;
LUMEX_PUBLIC_API std::string LumexLogging::s_launchTimestamp;

LUMEX_PUBLIC_API
void
LumexLogging::_log(LumexLogLevel level, std::string const &moduleName, std::string const &msg)
{
  {
    std::lock_guard<std::mutex> lock(s_mutex);

    // [timestamp] | LEVEL | module : message
    if(level >= LumexLogLevel::Debug && level <= LumexLogLevel::Warning)
    {
      std::clog << _levelToColor(level) << "[" << LumexTime::get_current_datetime() << "] "
                << "|" << std::setw(KLOG_WIDTH) << _levelToString(level) << "| " << moduleName << " : " << msg
                << "\033[0m"
                << "\n";
    }
    else
    {
      std::cerr << _levelToColor(level) << "[" << LumexTime::get_current_datetime() << "] "
                << "|" << std::setw(KLOG_WIDTH) << _levelToString(level) << "| " << moduleName << " : " << msg
                << "\033[0m"
                << "\n";
    }
  } // unlock mutex, because `toFile` locking it => avoid deadlock
  toFile(KDEFAULT_LOG_FILE_NAME, level, moduleName.c_str(), msg.c_str());
}

LUMEX_PUBLIC_API
char const *
LumexLogging::_levelToString(LumexLogLevel level) noexcept
{
  switch(level)
  {
  case LumexLogLevel::Debug: return "DEBUG";
  case LumexLogLevel::Info: return "INFO";
  case LumexLogLevel::Success: return "SUCCESS";
  case LumexLogLevel::Warning: return "WARNING";
  case LumexLogLevel::Error: return "ERROR";
  case LumexLogLevel::Critical: return "CRITICAL";
  default: return "UNKNOWN";
  }
}

LUMEX_PUBLIC_API
char const *
LumexLogging::_levelToColor(LumexLogLevel level) noexcept
{
  switch(level)
  {
  case LumexLogLevel::Debug: return "\033[36m";    // Cyan
  case LumexLogLevel::Info: return "\033[37m";     // White
  case LumexLogLevel::Success: return "\033[32m";  // Green
  case LumexLogLevel::Warning: return "\033[33m";  // Yellow
  case LumexLogLevel::Error: return "\033[31m";    // Red
  case LumexLogLevel::Critical: return "\033[41m"; // Red BG
  default: return "\033[0m";
  }
}

LUMEX_PUBLIC_API
Lumex::Path
LumexLogging::getLogsDirectory()
{
  try
  {
#if LUMEX_OS_UNIX
    // Check if we're running in AppImage mode
    std::string appImageMode    = LumexEnvironment::get("LUMEX_APPIMAGE_MODE").value;
    std::string externalLogsDir = LumexEnvironment::get("LUMEX_EXTERNAL_LOGS_DIR").value;

    if(!appImageMode.empty() && appImageMode == "1" && !externalLogsDir.empty())
    {
      Lumex::Path logsDir(externalLogsDir);
      Lumex::Filesystem::create_directory(logsDir);
      return logsDir;
    }

    // Default behavior for non-AppImage: use user's local data directory
    std::string homeDir = LumexEnvironment::get("HOME").value;
    if(homeDir.empty())
    {
      std::cerr << "Error: HOME environment variable not set, cannot "
                   "determine logs directory."
                << "\n";
      return Lumex::Filesystem::current_path().value_or(
        Lumex::Filesystem::temp_directory_path().value()); // Fallback, but likely to fail permissions
    }

    Lumex::Path userLogsDir = Lumex::Path(homeDir) / Lumex::Path(".local") / Lumex::Path("share")
                              / Lumex::Path("LumReportViewer") / Lumex::Path("logs");
    Lumex::Filesystem::create_directory(userLogsDir);
    return userLogsDir;
#else
    auto exePath = Lumex::Filesystem::get_exe_path();
    auto logsDir = exePath.parent_path() / Lumex::Path("logs");
    Lumex::Filesystem::create_directory(logsDir);
    return logsDir;
#endif
  }
  catch(std::exception const &e)
  {
    std::cerr << "Error initializing logs directory: " << e.what() << "\n";
    return Lumex::Filesystem::current_path().value_or(Lumex::Filesystem::temp_directory_path().value());
  }
}

LUMEX_PUBLIC_API
bool
LumexLogging::toFile(char const *filename, LumexLogLevel level, char const *moduleName, char const *msg,
                     bool appendTimestamp)
{
  try
  {
    std::lock_guard<std::mutex> lock(s_mutex);
    std::string logPath      = getLogsDirectory();
    std::string fullFilename = std::string(filename);

    if(appendTimestamp)
    {
      if(s_launchTimestamp.empty()) s_launchTimestamp = LumexTime::get_timestamp_ns();

      fullFilename = fullFilename + "_" + s_launchTimestamp + ".log";
    }
    else { fullFilename = fullFilename + ".log"; }

    Lumex::Path logFile = Lumex::Path(logPath) / Lumex::Path(fullFilename);
    std::ofstream file(logFile.c_str(), std::ios::app);
    if(!file.is_open())
    {
      std::cerr << "Failed to open log file: " << logFile << "\n";
      return false;
    }

    file << "[" << LumexTime::get_current_datetime() << "] "
         << "|" << std::setw(KLOG_WIDTH) << _levelToString(level) << "| " << moduleName << " : " << msg << "\n";

    return true;
  }
  catch(std::exception const &e)
  {
    std::cerr << "Error writing to log file: " << e.what() << "\n";
    return false;
  }
}
