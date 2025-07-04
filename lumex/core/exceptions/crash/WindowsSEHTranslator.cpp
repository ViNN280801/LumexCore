#define LUMEX_IMPLEMENTATION
#include "WindowsSEHTranslator.hpp"

#include "LumexCrashHandler.hpp"

inline void
seh_translator(LUMEX_ATTRIBUTE_MAYBE_UNUSED unsigned int code,
               _EXCEPTION_POINTERS *info)
{
  LumexCrashHandler::instance()._handleSEHException(info);
  TerminateProcess(GetCurrentProcess(), code);
}
