#ifndef LUMEX_LOGGING_MACRO_HPP
#define LUMEX_LOGGING_MACRO_HPP

#include "LumexLogging.hpp"

#define lumDebug(moduleName, ...) LumexLogging::debug(moduleName,  __VA_ARGS__)
#define lumInfo(moduleName, ...) LumexLogging::info(moduleName, __VA_ARGS__)
#define lumSuccess(moduleName, ...) LumexLogging::success(moduleName, __VA_ARGS__)
#define lumWarning(moduleName, ...) LumexLogging::warning(moduleName, __VA_ARGS__)
#define lumError(moduleName, ...) LumexLogging::error(moduleName, __VA_ARGS__)
#define lumCritical(moduleName, ...) LumexLogging::critical(moduleName, __VA_ARGS__)

#define lumDebugFL(moduleName, ...) LumexLogging::debug(moduleName, "[", __FILE__, ":", __LINE__, "] ", __VA_ARGS__)
#define lumInfoFL(moduleName, ...) LumexLogging::info(moduleName, "[", __FILE__, ":", __LINE__, "] ", __VA_ARGS__)
#define lumSuccessFL(moduleName, ...) LumexLogging::success(moduleName, "[", __FILE__, ":", __LINE__, "] ", __VA_ARGS__)
#define lumWarningFL(moduleName, ...) LumexLogging::warning(moduleName, "[", __FILE__, ":", __LINE__, "] ", __VA_ARGS__)
#define lumErrorFL(moduleName, ...) LumexLogging::error(moduleName, "[", __FILE__, ":", __LINE__, "] ", __VA_ARGS__)
#define lumCriticalFL(moduleName, ...) LumexLogging::critical(moduleName, "[", __FILE__, ":", __LINE__, "] ", __VA_ARGS__)

#endif // !LUMEX_LOGGING_MACRO_HPP
