#ifndef LUMEX_STACKTRACE_ENTRY_HPP
#define LUMEX_STACKTRACE_ENTRY_HPP

#include "lumex/LumexExport.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace Lumex
{
  namespace Core
  {
    namespace Stacktrace
    {
      /**
       * @brief Represents a single entry (frame) in a stacktrace
       * @details This class provides information about a single function call in the
       *          call stack, including the function name, source file, line number,
       *          and raw address. It follows the C++23 std::stacktrace_entry interface.
       *
       * @note The class is designed to be lightweight and copyable. All string
       *       operations are performed on-demand to minimize overhead during
       *       stacktrace capture.
       *
       * @example
       * LumexStacktraceEntry entry(reinterpret_cast<void*>(0x12345678));
       * std::cout << "Function: " << entry.description() << std::endl;
       * std::cout << "Source: " << entry.source_file() << ":" << entry.source_line() << std::endl;
       */
      class LUMEX_API LumexStacktraceEntry
      {
      public:
        using native_handle_type = void *;

      private:
        native_handle_type m_address;

// Suppress C4251 warnings for STL containers in DLL interface
#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251)
#endif
        // Cached values (mutable for lazy evaluation)
        mutable std::string m_cached_description;
        mutable std::string m_cached_source_file;
        mutable std::uint32_t m_cached_source_line;
        mutable bool m_cache_valid;
#ifdef _WIN32
  #pragma warning(pop)
#endif

        // Lazy initialization of cached values
        void ensure_cache_valid() const;

      public:
        /**
         * @brief Default constructor - creates an empty/invalid entry
         */
        LumexStacktraceEntry() noexcept
            : m_address(nullptr), m_cached_source_line(0), m_cache_valid(false)
        {}

        /**
         * @brief Constructs an entry from a native address
         * @param addr The address of the stack frame
         */
        explicit LumexStacktraceEntry(native_handle_type addr) noexcept
            : m_address(addr), m_cached_source_line(0), m_cache_valid(false)
        {}

        /**
         * @brief Copy constructor
         */
        LumexStacktraceEntry(LumexStacktraceEntry const &other) noexcept
            : m_address(other.m_address),
              m_cached_description(other.m_cached_description),
              m_cached_source_file(other.m_cached_source_file),
              m_cached_source_line(other.m_cached_source_line),
              m_cache_valid(other.m_cache_valid)
        {}

        /**
         * @brief Copy assignment operator
         */
        LumexStacktraceEntry &
        operator=(LumexStacktraceEntry const &other) noexcept
        {
          if(this != &other)
            {
              m_address            = other.m_address;
              m_cached_description = other.m_cached_description;
              m_cached_source_file = other.m_cached_source_file;
              m_cached_source_line = other.m_cached_source_line;
              m_cache_valid        = other.m_cache_valid;
            }
          return *this;
        }

        /**
         * @brief Destructor
         */
        ~LumexStacktraceEntry() = default;

        /**
         * @brief Returns the native handle (address) of this stack frame
         * @return The address as a platform-specific handle
         */
        constexpr native_handle_type
        native_handle() const noexcept
        {
          return m_address;
        }

        /**
         * @brief Checks if this entry is valid (non-null address)
         * @return true if the entry contains a valid address
         */
        constexpr explicit
        operator bool() const noexcept
        {
          return m_address != nullptr;
        }

        /**
         * @brief Gets a human-readable description of this stack frame
         * @details This typically includes the function name and offset, and may
         *          include source file and line information if available.
         * @return A string description of the frame
         *
         * @note This operation may be expensive on first call as it queries
         *       debug symbols
         */
        std::string description() const;

        /**
         * @brief Gets the source file name where this function is defined
         * @return The source file path, or empty string if unavailable
         *
         * @note This operation may be expensive on first call as it queries
         *       debug symbols
         */
        std::string source_file() const;

        /**
         * @brief Gets the source line number where this function call originated
         * @return The line number, or 0 if unavailable
         *
         * @note This operation may be expensive on first call as it queries
         *       debug symbols
         */
        std::uint32_t source_line() const;

        /**
         * @brief Comparison operators
         */
        friend bool
        operator==(LumexStacktraceEntry const &lhs,
                   LumexStacktraceEntry const &rhs) noexcept
        {
          return lhs.m_address == rhs.m_address;
        }

        friend bool
        operator!=(LumexStacktraceEntry const &lhs,
                   LumexStacktraceEntry const &rhs) noexcept
        {
          return !(lhs == rhs);
        }

        friend bool
        operator<(LumexStacktraceEntry const &lhs,
                  LumexStacktraceEntry const &rhs) noexcept
        {
          return lhs.m_address < rhs.m_address;
        }

        friend bool
        operator<=(LumexStacktraceEntry const &lhs,
                   LumexStacktraceEntry const &rhs) noexcept
        {
          return !(rhs < lhs);
        }

        friend bool
        operator>(LumexStacktraceEntry const &lhs,
                  LumexStacktraceEntry const &rhs) noexcept
        {
          return rhs < lhs;
        }

        friend bool
        operator>=(LumexStacktraceEntry const &lhs,
                   LumexStacktraceEntry const &rhs) noexcept
        {
          return !(lhs < rhs);
        }
      };

      /**
       * @brief Converts a stacktrace entry to a string
       * @param entry The entry to convert
       * @return A string representation of the entry
       */
      inline std::string
      to_string(LumexStacktraceEntry const &entry)
      {
        return entry.description();
      }

      /**
       * @brief Stream output operator for stacktrace entries
       */
      template <typename CharT, typename Traits>
      std::basic_ostream<CharT, Traits> &
      operator<<(std::basic_ostream<CharT, Traits> &oss,
                 LumexStacktraceEntry const &entry)
      {
        return oss << entry.description();
      }

    } // namespace Stacktrace
  } // namespace Core
} // namespace Lumex

// Hash support in std namespace (for C++11 compatibility)
namespace std
{
  template <> struct hash<Lumex::Core::Stacktrace::LumexStacktraceEntry> {
    size_t
    operator()(Lumex::Core::Stacktrace::LumexStacktraceEntry const &entry)
      const noexcept
    {
      return std::hash<void *>()(entry.native_handle());
    }
  };
}

#endif // LUMEX_STACKTRACE_ENTRY_HPP