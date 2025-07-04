#ifndef LUMEX_FILESYSTEM_EXPORT_HPP
#define LUMEX_FILESYSTEM_EXPORT_HPP

#ifdef _WIN32
  #ifdef LUMEX_FILESYSTEM_EXPORTS
    #define LUMEX_FILESYSTEM_API __declspec(dllexport)
  #else
    #define LUMEX_FILESYSTEM_API __declspec(dllimport)
  #endif
#else
  #define LUMEX_FILESYSTEM_API
#endif

#endif // !LUMEX_FILESYSTEM_EXPORT_HPP
