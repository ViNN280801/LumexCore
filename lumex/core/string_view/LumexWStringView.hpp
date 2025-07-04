#ifndef LUMEX_WSTRING_VIEW_HPP
#define LUMEX_WSTRING_VIEW_HPP

//  C++11 lightweight, non-owning view over wide character sequences.
//  Similar to std::wstring_view (C++17) but implemented manually for C++11.
//  The implementation is cross-platform (Windows / Linux / macOS) and relies
//  solely on the C++11 standard library.
//  It respects exception-safety, thread-safety (concurrent read-only access),
//  and does not perform dynamic allocation.

#include <cstddef>  // std::size_t, std::ptrdiff_t
#include <cstring>  // std::wcslen, std::wmemcmp
#include <iterator> // std::reverse_iterator
#include <ostream>  // std::wostream
#include <string>   // std::wstring

#include "lumex/LumexExport.hpp"

namespace Lumex
{
  namespace Core
  {
    namespace StringView
    {
      class LUMEX_API LumexWStringView
      {
      public:
        // — Public type aliases —
        using value_type       = wchar_t;
        using pointer          = wchar_t const *;
        using const_pointer    = wchar_t const *;
        using reference        = wchar_t const &;
        using const_reference  = wchar_t const &;
        using iterator         = const_pointer; // iterator == const_iterator
        using const_iterator   = const_pointer;
        using reverse_iterator = std::reverse_iterator<const_pointer>;
        using const_reverse_iterator = std::reverse_iterator<const_pointer>;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;

        static size_type const npos  = static_cast<size_type>(-1);

        // — Construction / assignment —
        constexpr LumexWStringView() noexcept : m_data(nullptr), m_size(0) {}

        // From C-string (constexpr friendly, does not throw)
        explicit LumexWStringView(wchar_t const *str) noexcept;

        // From pointer + size (does **not** check nullptr when len==0 on purpose)
        constexpr LumexWStringView(wchar_t const *str, size_type len) noexcept
            : m_data(str), m_size(len)
        {}

        // From std::wstring
        template <class Allocator>
        explicit LumexWStringView(
          std::basic_string<wchar_t, std::char_traits<wchar_t>,
                            Allocator> const &str) noexcept
            : m_data(str.data()), m_size(str.size())
        {}

        // enable construction from std::wstring by implicit instantiation of default
        // allocator
        explicit LumexWStringView(std::wstring const &str) noexcept;

        // Copy / assign — trivial
        constexpr LumexWStringView(LumexWStringView const &) noexcept
          = default;
        LumexWStringView &operator=(LumexWStringView const &) noexcept
          = default;

        // — Iterator support —
        constexpr const_iterator
        begin() const noexcept
        {
          return m_data;
        }
        constexpr const_iterator
        cbegin() const noexcept
        {
          return m_data;
        }
        constexpr const_iterator
        end() const noexcept
        {
          return m_data + m_size;
        }
        constexpr const_iterator
        cend() const noexcept
        {
          return m_data + m_size;
        }
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        // — Capacity —
        constexpr size_type
        size() const noexcept
        {
          return m_size;
        }
        constexpr size_type
        length() const noexcept
        {
          return m_size;
        }
        constexpr bool
        empty() const noexcept
        {
          return m_size == 0;
        }
        static constexpr size_type
        max_size() noexcept
        {
          return static_cast<size_type>(-1) / 2;
        }

        // — Element access —
        constexpr const_reference
        operator[](size_type idx) const noexcept
        {
          return m_data[idx];
        }
        const_reference at(size_type idx) const;

        constexpr const_reference
        front() const noexcept
        {
          return m_data[0];
        }
        constexpr const_reference
        back() const noexcept
        {
          return m_data[m_size - 1];
        }
        constexpr const_pointer
        data() const noexcept
        {
          return m_data;
        }

        // — Modifiers —
        void clear() noexcept;
        void remove_prefix(size_type n) noexcept;
        void remove_suffix(size_type n) noexcept;
        void swap(LumexWStringView &other) noexcept;

        // — Copy out —
        size_type
        copy(wchar_t *dest, size_type count, size_type pos = 0) const;

        // — Substring —
        LumexWStringView substr(size_type pos = 0, size_type n = npos) const;

        // — Comparison —
        int compare(LumexWStringView other) const noexcept;

        // convenience overloads
        int
        compare(size_type pos, size_type len, LumexWStringView other) const;

        int compare(wchar_t const *cstr) const;

        // — Starts / ends / contains helpers — (non-standard extensions but useful)
        bool starts_with(wchar_t chr) const noexcept;
        bool starts_with(LumexWStringView str) const noexcept;
        bool ends_with(wchar_t chr) const noexcept;
        bool ends_with(LumexWStringView str) const noexcept;

        // — Find (simple implementations) —
        size_type find(wchar_t chr, size_type pos = 0) const noexcept;

        size_type find(LumexWStringView str, size_type pos = 0) const noexcept;

        size_type find(wchar_t const *cstr, size_type pos,
                       size_type count) const noexcept;

        size_type find(wchar_t const *cstr, size_type pos = 0) const noexcept;

        // — Reverse find —
        size_type
        rfind(LumexWStringView str, size_type pos = npos) const noexcept;

        size_type rfind(wchar_t chr, size_type pos = npos) const noexcept;

        size_type rfind(wchar_t const *cstr, size_type pos,
                        size_type count) const noexcept;

        size_type
        rfind(wchar_t const *cstr, size_type pos = npos) const noexcept;

        // — Find first of —
        size_type
        find_first_of(LumexWStringView str, size_type pos = 0) const noexcept;

        size_type find_first_of(wchar_t chr, size_type pos = 0) const noexcept;

        size_type find_first_of(wchar_t const *cstr, size_type pos,
                                size_type count) const noexcept;

        size_type
        find_first_of(wchar_t const *cstr, size_type pos = 0) const noexcept;

        // — Find last of —
        size_type find_last_of(LumexWStringView str,
                               size_type pos = npos) const noexcept;

        size_type
        find_last_of(wchar_t chr, size_type pos = npos) const noexcept;

        size_type find_last_of(wchar_t const *cstr, size_type pos,
                               size_type count) const noexcept;

        size_type
        find_last_of(wchar_t const *cstr, size_type pos = npos) const noexcept;

        // — Find first not of —
        size_type find_first_not_of(LumexWStringView str,
                                    size_type pos = 0) const noexcept;

        size_type
        find_first_not_of(wchar_t chr, size_type pos = 0) const noexcept;

        size_type find_first_not_of(wchar_t const *cstr, size_type pos,
                                    size_type count) const noexcept;

        size_type find_first_not_of(wchar_t const *cstr,
                                    size_type pos = 0) const noexcept;

        // — Find last not of —
        size_type find_last_not_of(LumexWStringView str,
                                   size_type pos = npos) const noexcept;

        size_type
        find_last_not_of(wchar_t chr, size_type pos = npos) const noexcept;

        size_type find_last_not_of(wchar_t const *cstr, size_type pos,
                                   size_type count) const noexcept;

        size_type find_last_not_of(wchar_t const *cstr,
                                   size_type pos = npos) const noexcept;

        // — Conversion back to std::wstring —
        template <class Allocator = std::allocator<wchar_t>>
        explicit
        operator std::basic_string<wchar_t, std::char_traits<wchar_t>,
                                   Allocator>() const
        {
          return std::basic_string<wchar_t, std::char_traits<wchar_t>,
                                   Allocator>(m_data, m_size);
        }

        template <class Allocator = std::allocator<wchar_t>>
        std::basic_string<wchar_t, std::char_traits<wchar_t>, Allocator>
        to_string(Allocator const &alloc = Allocator()) const
        {
          return std::basic_string<wchar_t, std::char_traits<wchar_t>,
                                   Allocator>(m_data, m_size, alloc);
        }

      private:
        wchar_t const *m_data;
        size_type m_size;
      };

      // — Non-member relational operators —
      LUMEX_API inline bool
      operator==(LumexWStringView lhs, LumexWStringView rhs) noexcept
      {
        return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
      }
      LUMEX_API inline bool
      operator!=(LumexWStringView lhs, LumexWStringView rhs) noexcept
      {
        return !(lhs == rhs);
      }
      LUMEX_API inline bool
      operator<(LumexWStringView lhs, LumexWStringView rhs) noexcept
      {
        return lhs.compare(rhs) < 0;
      }
      LUMEX_API inline bool
      operator>(LumexWStringView lhs, LumexWStringView rhs) noexcept
      {
        return lhs.compare(rhs) > 0;
      }
      LUMEX_API inline bool
      operator<=(LumexWStringView lhs, LumexWStringView rhs) noexcept
      {
        return lhs.compare(rhs) <= 0;
      }
      LUMEX_API inline bool
      operator>=(LumexWStringView lhs, LumexWStringView rhs) noexcept
      {
        return lhs.compare(rhs) >= 0;
      }

      // — Stream inserter —
      LUMEX_API std::wostream &
      operator<<(std::wostream &wostr, LumexWStringView wsview);

    } // namespace StringView
  } // namespace Core
} // namespace Lumex

using LumexWStringView = Lumex::Core::StringView::LumexWStringView;

#endif // LUMEX_WSTRING_VIEW_HPP
