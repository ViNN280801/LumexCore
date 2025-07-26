#ifndef LUMEX_STACKTRACE_HPP
#define LUMEX_STACKTRACE_HPP

#include "lumex/core/exceptions/stacktrace/LumexStacktraceEntry.hpp"
#include "lumex/core/utility/LumexUtility"

#include <algorithm>
#include <cstring>
#include <functional>
#include <memory>
#include <string>
#include <vector>

// ****************** Platform-specific includes ****************** //
#if LUMEX_OS_WINDOWS
  #include <windows.h> // This include should be 1st

  #include <DbgHelp.h> // This include should be 2nd, because it uses types from windows.h

  #include <mutex>

  #pragma comment(lib, "dbghelp.lib")
#else
  #include <cstdio>
  #include <cstdlib>
  #include <cxxabi.h>
  #include <dlfcn.h>
  #include <execinfo.h>
  #include <sstream>

  #if defined(__linux__)
    #include <sys/wait.h>
    #include <unistd.h>
  #endif
#endif

// *********************************************************************** //

namespace Lumex // NOLINT(modernize-concat-nested-namespaces)
{
  namespace Core
  {
    namespace Stacktrace
    {
      template <typename Allocator> class LumexBasicStacktrace;

      namespace detail
      {
        constexpr short const kHashRightShift     = 2;
        constexpr short const kHashLeftShift      = 6;
        constexpr short const kDefaultAddrStrSize = 32;
        constexpr short const kDefaultMaxFrames   = 128;
        constexpr short const kDefaultBufferSize  = 256;
        constexpr short const kDefaultCmdSize     = 512;
        constexpr std::size_t kHashGoldenRatio    = 0x9e3779b9U;

#if LUMEX_OS_WINDOWS
        // DbgHelp is not thread-safe, so we need a mutex
        extern std::mutex g_dbghelp_mutex;

        // Helper class for RAII DbgHelp initialization
        class DbgHelpInitializer
        {
        private:
          static bool s_initialized;

        public:
          DbgHelpInitializer();
          ~DbgHelpInitializer() = default;
          bool is_initialized() const noexcept;
        };

        /**
         * @brief Platform-specific stacktrace capture implementation
         * @details This function captures the current call stack using platform-specific
         *          APIs (DbgHelp on Windows, backtrace on Unix).
         *
         * @tparam Allocator The allocator type for the resulting stacktrace
         * @param skip Number of frames to skip from the top
         * @param max_depth Maximum number of frames to capture
         * @param alloc The allocator to use
         * @return A LumexBasicStacktrace containing the captured frames
         */
        template <typename Allocator>
        Stacktrace::LumexBasicStacktrace<Allocator>
        capture_stacktrace(size_t skip, size_t max_depth, Allocator const &alloc) noexcept;

        bool resolve_symbol_info(void *address, std::string &function_name, std::string &source_file,
                                 std::uint32_t &line_number) noexcept;
#else
        std::string demangle_symbol(char const *mangled);

        bool get_source_info_addr2line(void *address, std::string &file, std::uint32_t &line);

        template <typename Allocator>
        Stacktrace::LumexBasicStacktrace<Allocator>
        capture_stacktrace(size_t skip, size_t max_depth, Allocator const &alloc) noexcept;

        bool resolve_symbol_info(void *address, std::string &function_name, std::string &source_file,
                                 std::uint32_t &line_number) noexcept;
#endif
      } // namespace detail

// Suppress C4251 warnings for STL containers in DLL interface for the entire template class
#ifdef _WIN32
  #pragma warning(push)
  #pragma warning(disable : 4251)
#endif

      template <typename Allocator = std::allocator<LumexStacktraceEntry>> class LumexBasicStacktrace
      {
        template <typename A>
        friend LumexBasicStacktrace<A> detail::capture_stacktrace(size_t, size_t, A const &) noexcept;

      public:
        using value_type             = LumexStacktraceEntry;
        using allocator_type         = Allocator;
        using size_type              = typename std::allocator_traits<Allocator>::size_type;
        using difference_type        = typename std::allocator_traits<Allocator>::difference_type;
        using reference              = value_type &;
        using const_reference        = value_type const &;
        using pointer                = typename std::allocator_traits<Allocator>::pointer;
        using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;

        using container_type         = std::vector<value_type, allocator_type>;

        using iterator               = typename container_type::const_iterator;
        using const_iterator         = typename container_type::const_iterator;
        using reverse_iterator       = typename container_type::const_reverse_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;

      private:
        container_type m_entries;

        explicit LumexBasicStacktrace(container_type &&entries) : m_entries(std::move(entries)) {}

      public:
        LumexBasicStacktrace() noexcept(noexcept(container_type())) = default;

        explicit LumexBasicStacktrace(allocator_type const &alloc) noexcept : m_entries(alloc) {}

        LumexBasicStacktrace(LumexBasicStacktrace const &)                = default;
        LumexBasicStacktrace(LumexBasicStacktrace &&) noexcept            = default;
        LumexBasicStacktrace &operator=(LumexBasicStacktrace const &)     = default;
        LumexBasicStacktrace &operator=(LumexBasicStacktrace &&) noexcept = default;
        ~LumexBasicStacktrace()                                           = default;

        static LumexBasicStacktrace
        current(size_type skip = 1, size_type max_depth = static_cast<size_type>(-1),
                allocator_type const &alloc = allocator_type()) noexcept
        {
          return detail::capture_stacktrace<Allocator>(skip + 1, max_depth, alloc);
        }

        allocator_type
        get_allocator() const noexcept
        {
          return m_entries.get_allocator();
        }

        const_iterator
        begin() const noexcept
        {
          return m_entries.begin();
        }
        const_iterator
        end() const noexcept
        {
          return m_entries.end();
        }
        const_iterator
        cbegin() const noexcept
        {
          return m_entries.cbegin();
        }
        const_iterator
        cend() const noexcept
        {
          return m_entries.cend();
        }
        const_reverse_iterator
        rbegin() const noexcept
        {
          return m_entries.rbegin();
        }
        const_reverse_iterator
        rend() const noexcept
        {
          return m_entries.rend();
        }
        const_reverse_iterator
        crbegin() const noexcept
        {
          return m_entries.crbegin();
        }
        const_reverse_iterator
        crend() const noexcept
        {
          return m_entries.crend();
        }

        bool
        empty() const noexcept
        {
          return m_entries.empty();
        }
        size_type
        size() const noexcept
        {
          return m_entries.size();
        }
        size_type
        max_size() const noexcept
        {
          return m_entries.max_size();
        }

        const_reference
        operator[](size_type pos) const
        {
          return m_entries[pos];
        }
        const_reference
        at(size_type pos) const
        {
          return m_entries.at(pos);
        }

        void
        swap(LumexBasicStacktrace &other) noexcept(noexcept(m_entries.swap(other.m_entries)))
        {
          m_entries.swap(other.m_entries);
        }
      };

#ifdef _WIN32
  #pragma warning(pop)
#endif

      using LumexStacktrace = LumexBasicStacktrace<std::allocator<LumexStacktraceEntry>>;

      template <typename Allocator1, typename Allocator2>
      bool
      operator==(LumexBasicStacktrace<Allocator1> const &lhs, LumexBasicStacktrace<Allocator2> const &rhs) noexcept
      {
        if(lhs.size() != rhs.size()) return false;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
      }

      template <typename Allocator1, typename Allocator2>
      bool
      operator!=(LumexBasicStacktrace<Allocator1> const &lhs, LumexBasicStacktrace<Allocator2> const &rhs) noexcept
      {
        return !(lhs == rhs);
      }

      template <typename Allocator1, typename Allocator2>
      bool
      operator<(LumexBasicStacktrace<Allocator1> const &lhs, LumexBasicStacktrace<Allocator2> const &rhs) noexcept
      {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
      }

      template <typename Allocator1, typename Allocator2>
      bool
      operator<=(LumexBasicStacktrace<Allocator1> const &lhs, LumexBasicStacktrace<Allocator2> const &rhs) noexcept
      {
        return !(rhs < lhs);
      }

      template <typename Allocator1, typename Allocator2>
      bool
      operator>(LumexBasicStacktrace<Allocator1> const &lhs, LumexBasicStacktrace<Allocator2> const &rhs) noexcept
      {
        return rhs < lhs;
      }

      template <typename Allocator1, typename Allocator2>
      bool
      operator>=(LumexBasicStacktrace<Allocator1> const &lhs, LumexBasicStacktrace<Allocator2> const &rhs) noexcept
      {
        return !(lhs < rhs);
      }

      template <typename Allocator>
      void
      swap(LumexBasicStacktrace<Allocator> &lhs, LumexBasicStacktrace<Allocator> &rhs) noexcept(noexcept(lhs.swap(rhs)))
      {
        lhs.swap(rhs);
      }

      template <typename Allocator>
      std::string
      to_string(LumexBasicStacktrace<Allocator> const &stacktrace)
      {
        std::string result;
        result.reserve(stacktrace.size() * detail::kDefaultMaxFrames);

        for(size_t i = 0; i < stacktrace.size(); ++i)
        {
          result += std::to_string(i);
          result += "# ";
          result += stacktrace[i].description();
          result += '\n';
        }

        return result;
      }

      template <typename CharT, typename Traits, typename Allocator>
      std::basic_ostream<CharT, Traits> &
      operator<<(std::basic_ostream<CharT, Traits> &ostream, LumexBasicStacktrace<Allocator> const &stacktrace)
      {
        return ostream << to_string(stacktrace);
      }

      template <typename Allocator> struct hash;

      template <typename Allocator> struct hash<LumexBasicStacktrace<Allocator>> {
        size_t
        operator()(LumexBasicStacktrace<Allocator> const &stacktrace) const noexcept
        {
          size_t seed = 0;
          for(auto const &entry : stacktrace)
          {
            seed ^= std::hash<void *>()(entry.native_handle()) + detail::kHashGoldenRatio
                    + (seed << detail::kHashLeftShift) + (seed >> detail::kHashRightShift);
          }
          return seed;
        }
      };
    } // namespace Stacktrace
  } // namespace Core
} // namespace Lumex

// Global type aliases for convenience
using LumexStacktrace      = Lumex::Core::Stacktrace::LumexStacktrace;
using LumexStacktraceEntry = Lumex::Core::Stacktrace::LumexStacktraceEntry;

// Bring key functions into global namespace for convenience
using Lumex::Core::Stacktrace::to_string;

#endif // !LUMEX_STACKTRACE_HPP
