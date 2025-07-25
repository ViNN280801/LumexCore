#ifndef LUMEX_DEMANGLE_HPP
#define LUMEX_DEMANGLE_HPP

/**
 * @brief Demangles a C++ type name for human-readable output.
 *
 * This method takes a mangled C++ type name (as returned by typeid) and attempts
 * to demangle it into a human-readable format. On GCC/Clang platforms, it uses
 * the ABI's demangling function. On other platforms, it returns the original name.
 *
 * @param[in] name The mangled type name to demangle (typically from typeid().name())
 * @return std::string The demangled name if successful, or the original name if:
 *         - Platform doesn't support demangling (non-GNU)
 *         - Demangling failed
 *         - Input was nullptr
 *
 * @note On GNU-compatible compilers (GCC/Clang), this uses abi::__cxa_demangle
 *       which handles:
 *       - Template instantiations
 *       - Namespace qualifications
 *       - CV-qualifiers
 *       - Calling conventions
 *
 * @warning The caller must ensure the input pointer is valid (not dangling).
 *          The method makes no ownership claims on the input string.
 *
 * @example
 *   std::cout << _demangle(typeid(std::vector<int>).name());
 *   // Outputs: "std::vector<int, std::allocator<int>>" instead of "St6vectorIiSaIiEE"
 */
#ifdef __GNUG__
  #include <cstdlib>
  #include <cxxabi.h>
  #include <string>
  #include <typeinfo>

  #define lumDemangle(type) \
    ([&]() -> std::string { \
      std::string name = typeid(type).name(); \
      int status = 0; \
      char *demangled = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status); \
      std::string result = (status == 0 && demangled) ? demangled : name; \
      free(demangled); \
      return result; \
    }())
#else
  #include <string>
  #include <typeinfo>

  #define lumDemangle(type) std::string(typeid(type).name())
#endif

#endif // !LUMEX_DEMANGLE_HPP
