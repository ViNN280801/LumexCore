#ifndef LUMEX_OPTIONAL_HPP
#define LUMEX_OPTIONAL_HPP

//  C++11 implementation of std::optional (C++17 feature)
//  Provides a type-safe nullable value container that either holds a value
//  or is empty. This implementation is cross-platform and uses only C++11
//  standard library features.
//  Exception-safe, supports move semantics, and provides full std::optional
//  API.

#include <functional>       // std::hash
#include <initializer_list> // std::initializer_list
#include <stdexcept>        // std::logic_error
#include <type_traits>      // std::aligned_storage, std::is_*
#include <utility>          // std::forward, std::move

namespace Lumex
{
  namespace Core
  {
    namespace Optional
    {
      // — nullopt_t and nullopt —
      struct nullopt_t {
        struct init_tag {};
        explicit constexpr nullopt_t(init_tag /*unused*/) {}
      };
      constexpr nullopt_t nullopt{nullopt_t::init_tag{}};

      // — in_place_t for in-place construction —
      struct in_place_t {
        struct init_tag {};
        explicit constexpr in_place_t(init_tag /*unused*/) {}
      };
      constexpr in_place_t in_place{in_place_t::init_tag{}};

      // — Exception type —
      class LumexBadOptionalAccess : public std::logic_error
      {
      public:
        LumexBadOptionalAccess()
            : std::logic_error("LumexBadOptionalAccess: Bad optional access")
        {}
        explicit LumexBadOptionalAccess(char const *what_arg)
            : std::logic_error(what_arg)
        {}
        explicit LumexBadOptionalAccess(std::string const &what_arg)
            : std::logic_error(what_arg)
        {}
      };

      // — Main optional class —
      template <typename T> class LumexOptional
      {
      public:
        // — Type aliases —
        using value_type = T;

      private:
        // — Storage management —
        typename std::aligned_storage<sizeof(T), alignof(T)>::type m_storage;
        bool m_has_value;

        // — Internal access helpers —
        T *
        get_ptr() noexcept
        {
          return reinterpret_cast<T *>(&m_storage);
        }

        T const *
        get_ptr() const noexcept
        {
          return reinterpret_cast<T const *>(&m_storage);
        }

// — Internal construction/destruction —
// suppress MSVC "unreachable code" in construct()
#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable : 4702)
#endif
        template <typename... Args>
        void
        construct(Args &&...args)
        {
          new(get_ptr()) T(std::forward<Args>(args)...);
          m_has_value = true;
        }
#ifdef _MSC_VER
  #pragma warning(pop)
#endif

        void
        destroy() noexcept
        {
          if(m_has_value)
            {
              get_ptr()->~T();
              m_has_value = false;
            }
        }

      public:
        // — Constructors —

        // Default constructor (empty optional)
        constexpr LumexOptional() noexcept : m_has_value(false) {}

        // nullopt constructor
        constexpr LumexOptional(nullopt_t /*unused*/) noexcept
            : m_has_value(false)
        {}

        // Copy constructor
        LumexOptional(LumexOptional const &other) noexcept(
          std::is_nothrow_copy_constructible<T>::value)
            : m_has_value(false)
        {
          if(other.m_has_value) construct(*other.get_ptr());
        }

        // Move constructor
        LumexOptional(LumexOptional &&other) noexcept(
          std::is_nothrow_move_constructible<T>::value)
            : m_has_value(false)
        {
          if(other.m_has_value)
            {
              construct(std::move(*other.get_ptr()));
              other.destroy();
            }
        }

        // Value copy constructor
        LumexOptional(T const &value) noexcept(
          std::is_nothrow_copy_constructible<T>::value)
            : m_has_value(false)
        {
          construct(value);
        }

        // Value move constructor
        LumexOptional(T &&value) noexcept(
          std::is_nothrow_move_constructible<T>::value)
            : m_has_value(false)
        {
          construct(std::move(value));
        }

        // In-place constructor
        template <typename... Args>
        explicit LumexOptional(in_place_t /*unused*/, Args &&...args)
            : m_has_value(false)
        {
          construct(std::forward<Args>(args)...);
        }

        // In-place constructor with initializer_list
        template <typename U, typename... Args>
        explicit LumexOptional(in_place_t /*unused*/,
                               std::initializer_list<U> ilist, Args &&...args)
            : m_has_value(false)
        {
          construct(ilist, std::forward<Args>(args)...);
        }

        // — Destructor —
        ~LumexOptional() { destroy(); }

        // — Assignment operators —

        // nullopt assignment
        LumexOptional &
        operator=(nullopt_t /*unused*/) noexcept
        {
          destroy();
          return *this;
        }

        // Copy assignment
        LumexOptional &
        operator=(LumexOptional const &other) noexcept(
          std::is_nothrow_copy_constructible<T>::value
          && std::is_nothrow_copy_assignable<T>::value)
        {
          if(this != &other)
            {
              if(other.m_has_value) // 'other' has a value
                {
                  if(m_has_value) // '*this' also has a value: destroy old, construct new
                    {
                      destroy();
                      construct(*other.get_ptr());
                    }
                  else // '*this' is empty, 'other' has value: construct new
                    {
                      construct(*other.get_ptr());
                    }
                }
              else // 'other' is empty
                {
                  if(m_has_value) // '*this' has a value, 'other' is empty: destroy '*this' value
                    {
                      destroy(); // This is the line that previously caused the warning.
                    }
                  // else: Both are empty, nothing to do.
                }
            }
          return *this;
        }

        // Move assignment
        LumexOptional &
        operator=(LumexOptional &&other) noexcept(
          std::is_nothrow_move_constructible<T>::value
          && std::is_nothrow_move_assignable<T>::value)
        {
          if(this != &other)
            {
              if(other.m_has_value)
                {
                  if(m_has_value)
                    **this = std::move(*other);
                  else
                    construct(std::move(*other.get_ptr()));
                  other.destroy();
                }
              else { destroy(); }
            }
          return *this;
        }

        // Value copy assignment
        template <typename U = T>
        typename std::enable_if<
          std::is_same<typename std::decay<U>::type, T>::value,
          LumexOptional &>::type
        operator=(U &&value)
        {
          if(m_has_value)
            **this = std::forward<U>(value);
          else
            construct(std::forward<U>(value));
          return *this;
        }

        // — Observers —

        // Dereference operators
        T const *
        operator->() const noexcept
        {
          return get_ptr();
        }

        T *
        operator->() noexcept
        {
          return get_ptr();
        }

        T const &
        operator*() const & noexcept
        {
          return *get_ptr();
        }

        T &
        operator*() & noexcept
        {
          return *get_ptr();
        }

        T const &&
        operator*() const && noexcept
        {
          return std::move(*get_ptr());
        }

        T &&
        operator*() && noexcept
        {
          return std::move(*get_ptr());
        }

        // Boolean conversion
        explicit
        operator bool() const noexcept
        {
          return m_has_value;
        }

        // has_value() method
        bool
        has_value() const noexcept
        {
          return m_has_value;
        }

        // value() with exception
        T &
        value() &
        {
          if(!m_has_value) throw LumexBadOptionalAccess();
          return *get_ptr();
        }

        T const &
        value() const &
        {
          if(!m_has_value) throw LumexBadOptionalAccess();
          return *get_ptr();
        }

        T &&
        value() &&
        {
          if(!m_has_value) throw LumexBadOptionalAccess();
          return std::move(*get_ptr());
        }

        T const &&
        value() const &&
        {
          if(!m_has_value) throw LumexBadOptionalAccess();
          return std::move(*get_ptr());
        }

        // value_or() methods
        template <typename U>
        T
        value_or(U &&default_value) const &
        {
          return m_has_value ? **this
                             : static_cast<T>(std::forward<U>(default_value));
        }

        template <typename U>
        T
        value_or(U &&default_value) &&
        {
          return m_has_value ? std::move(**this)
                             : static_cast<T>(std::forward<U>(default_value));
        }

        // — Modifiers —

        // swap
        void
        swap(LumexOptional &other) noexcept(
          std::is_nothrow_move_constructible<T>::value
          && std::is_nothrow_move_assignable<T>::value)
        {
          if(m_has_value && other.m_has_value)
            {
              using std::swap;
              swap(**this, *other);
            }
          else if(m_has_value)
            {
              other.construct(std::move(**this));
              destroy();
            }
          else if(other.m_has_value)
            {
              construct(std::move(*other));
              other.destroy();
            }
        }

        // reset
        void
        reset() noexcept
        {
          destroy();
        }

// emplace
#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable : 4702)
#endif
        template <typename... Args>
        T &
        emplace(Args &&...args)
        {
          destroy();
          construct(std::forward<Args>(args)...);
          return **this;
        }

        // suppress MSVC "unreachable code" in emplace(initializer_list<>)
        template <typename U, typename... Args>
        T &
        emplace(std::initializer_list<U> ilist, Args &&...args)
        {
          destroy();
          construct(ilist, std::forward<Args>(args)...);
          return **this;
        }
#ifdef _MSC_VER
  #pragma warning(pop)
#endif
      };

      // — Non-member comparison operators —

      // Comparison between optionals

      // non-constexpr, because clang-tidy says that constexpr for this function is
      // C++14 extension Use of this statement in a constexpr function is a C++14
      // extensionclang(-Wc++14-extensions)
      template <typename T, typename U>
      bool
      operator==(LumexOptional<T> const &lhs, LumexOptional<U> const &rhs)
      {
        if(lhs.has_value() != rhs.has_value()) return false;
        if(!lhs.has_value()) return true; // both are empty
        return *lhs == *rhs;
      }

      template <typename T, typename U>
      constexpr bool
      operator!=(LumexOptional<T> const &lhs, LumexOptional<U> const &rhs)
      {
        return !(lhs == rhs);
      }

      // non-constexpr, because clang-tidy says that constexpr for this function is
      // C++14 extension Use of this statement in a constexpr function is a C++14
      // extensionclang(-Wc++14-extensions)
      template <typename T, typename U>
      bool
      operator<(LumexOptional<T> const &lhs, LumexOptional<U> const &rhs)
      {
        if(!rhs.has_value()) return false; // nothing is less than empty
        if(!lhs.has_value()) return true;  // empty is less than any value
        return *lhs < *rhs;
      }

      template <typename T, typename U>
      constexpr bool
      operator<=(LumexOptional<T> const &lhs, LumexOptional<U> const &rhs)
      {
        return !(rhs < lhs);
      }

      template <typename T, typename U>
      constexpr bool
      operator>(LumexOptional<T> const &lhs, LumexOptional<U> const &rhs)
      {
        return rhs < lhs;
      }

      template <typename T, typename U>
      constexpr bool
      operator>=(LumexOptional<T> const &lhs, LumexOptional<U> const &rhs)
      {
        return !(lhs < rhs);
      }

      // Comparison with nullopt
      template <typename T>
      constexpr bool
      operator==(LumexOptional<T> const &opt, nullopt_t /*unused*/) noexcept
      {
        return !opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator==(nullopt_t /*unused*/, LumexOptional<T> const &opt) noexcept
      {
        return !opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator!=(LumexOptional<T> const &opt, nullopt_t /*unused*/) noexcept
      {
        return opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator!=(nullopt_t /*unused*/, LumexOptional<T> const &opt) noexcept
      {
        return opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator<(LumexOptional<T> const & /*unused*/,
                nullopt_t /*unused*/) noexcept
      {
        return false;
      }

      template <typename T>
      constexpr bool
      operator<(nullopt_t /*unused*/, LumexOptional<T> const &opt) noexcept
      {
        return opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator<=(LumexOptional<T> const &opt, nullopt_t /*unused*/) noexcept
      {
        return !opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator<=(nullopt_t /*unused*/,
                 LumexOptional<T> const & /*unused*/) noexcept
      {
        return true;
      }

      template <typename T>
      constexpr bool
      operator>(LumexOptional<T> const &opt, nullopt_t /*unused*/) noexcept
      {
        return opt.has_value();
      }

      template <typename T>
      constexpr bool
      operator>(nullopt_t /*unused*/,
                LumexOptional<T> const & /*unused*/) noexcept
      {
        return false;
      }

      template <typename T>
      constexpr bool
      operator>=(LumexOptional<T> const & /*unused*/,
                 nullopt_t /*unused*/) noexcept
      {
        return true;
      }

      template <typename T>
      constexpr bool
      operator>=(nullopt_t /*unused*/, LumexOptional<T> const &opt) noexcept
      {
        return !opt.has_value();
      }

      // Comparison with values
      template <typename T, typename U>
      constexpr bool
      operator==(LumexOptional<T> const &opt, U const &value)
      {
        return opt.has_value() ? *opt == value : false;
      }

      template <typename T, typename U>
      constexpr bool
      operator==(T const &value, LumexOptional<U> const &opt)
      {
        return opt.has_value() ? value == *opt : false;
      }

      template <typename T, typename U>
      constexpr bool
      operator!=(LumexOptional<T> const &opt, U const &value)
      {
        return opt.has_value() ? *opt != value : true;
      }

      template <typename T, typename U>
      constexpr bool
      operator!=(T const &value, LumexOptional<U> const &opt)
      {
        return opt.has_value() ? value != *opt : true;
      }

      template <typename T, typename U>
      constexpr bool
      operator<(LumexOptional<T> const &opt, U const &value)
      {
        return opt.has_value() ? *opt < value : true;
      }

      template <typename T, typename U>
      constexpr bool
      operator<(T const &value, LumexOptional<U> const &opt)
      {
        return opt.has_value() ? value < *opt : false;
      }

      template <typename T, typename U>
      constexpr bool
      operator<=(LumexOptional<T> const &opt, U const &value)
      {
        return opt.has_value() ? *opt <= value : true;
      }

      template <typename T, typename U>
      constexpr bool
      operator<=(T const &value, LumexOptional<U> const &opt)
      {
        return opt.has_value() ? value <= *opt : false;
      }

      template <typename T, typename U>
      constexpr bool
      operator>(LumexOptional<T> const &opt, U const &value)
      {
        return opt.has_value() ? *opt > value : false;
      }

      template <typename T, typename U>
      constexpr bool
      operator>(T const &value, LumexOptional<U> const &opt)
      {
        return opt.has_value() ? value > *opt : true;
      }

      template <typename T, typename U>
      constexpr bool
      operator>=(LumexOptional<T> const &opt, U const &value)
      {
        return opt.has_value() ? *opt >= value : false;
      }

      template <typename T, typename U>
      constexpr bool
      operator>=(T const &value, LumexOptional<U> const &opt)
      {
        return opt.has_value() ? value >= *opt : true;
      }

      // — Specialized algorithms —

      // swap
      template <typename T>
      void
      swap(LumexOptional<T> &lhs,
           LumexOptional<T> &rhs) noexcept(noexcept(lhs.swap(rhs)))
      {
        lhs.swap(rhs);
      }

      // make_optional
      template <typename T>
      constexpr LumexOptional<typename std::decay<T>::type>
      make_optional(T &&value)
      {
        return LumexOptional<typename std::decay<T>::type>(
          std::forward<T>(value));
      }

      template <typename T, typename... Args>
      constexpr LumexOptional<T>
      make_optional(Args &&...args)
      {
        return LumexOptional<T>(in_place, std::forward<Args>(args)...);
      }

      template <typename T, typename U, typename... Args>
      constexpr LumexOptional<T>
      make_optional(std::initializer_list<U> ilist, Args &&...args)
      {
        return LumexOptional<T>(in_place, ilist, std::forward<Args>(args)...);
      }

    } // namespace Optional
  } // namespace Core
} // namespace Lumex

using Lumex::Core::Optional::in_place;
using Lumex::Core::Optional::LumexBadOptionalAccess;
using Lumex::Core::Optional::make_optional;
using Lumex::Core::Optional::nullopt;

template <typename T>
using LumexOptional = Lumex::Core::Optional::LumexOptional<T>;

// — Hash support —
namespace std
{
  template <typename T> struct hash<LumexOptional<T>> {
    size_t
    operator()(LumexOptional<T> const &opt) const
    {
      return opt.has_value() ? hash<T>{}(*opt) : 0;
    }
  };
} // namespace std

#endif // !LUMEX_OPTIONAL_HPP
