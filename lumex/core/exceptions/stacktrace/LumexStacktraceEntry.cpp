#define LUMEX_IMPLEMENTATION
#include "LumexStacktraceEntry.hpp"
#include "lumex/LumexExport.hpp"

// Forward declaration for resolve_symbol_info from LumexStacktrace.hpp
namespace Lumex // NOLINT(modernize-concat-nested-namespaces)
{
  namespace Core
  {
    namespace Stacktrace
    {
      namespace detail
      {
        bool resolve_symbol_info(void *address, std::string &function_name, std::string &source_file,
                                 std::uint32_t &line_number) noexcept;
      }
    }
  }
}

namespace Lumex // NOLINT(modernize-concat-nested-namespaces)
{
  namespace Core
  {
    namespace Stacktrace
    {
      LUMEX_PUBLIC_API
      void
      LumexStacktraceEntry::ensure_cache_valid() const
      {
        if(m_cache_valid || m_address == nullptr) return;

        detail::resolve_symbol_info(m_address, m_cached_description, m_cached_source_file, m_cached_source_line);

        m_cache_valid = true;
      }

      LUMEX_PUBLIC_API
      std::string
      LumexStacktraceEntry::description() const
      {
        ensure_cache_valid();
        return m_cached_description;
      }

      LUMEX_PUBLIC_API
      std::string
      LumexStacktraceEntry::source_file() const
      {
        ensure_cache_valid();
        return m_cached_source_file;
      }

      LUMEX_PUBLIC_API
      std::uint32_t
      LumexStacktraceEntry::source_line() const
      {
        ensure_cache_valid();
        return m_cached_source_line;
      }

    } // namespace Stacktrace
  } // namespace Core
} // namespace Lumex
