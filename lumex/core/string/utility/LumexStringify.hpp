#ifndef LUMEX_UTILITIES_HPP
#define LUMEX_UTILITIES_HPP

#include <sstream>
#include <string>
#include <type_traits>

namespace Lumex
{
  namespace String
  {
    namespace Utility
    {
      // C++14 doesn't have std::void_t
      template <typename... T>
      using void_t = void; // NOLINT(readability-identifier-naming)

      /**
       * @brief Check if a type is streamable, that is, if it can be inserted into an ostream.
       * @tparam T Type to check.
       * @tparam Enable Enable parameter to enable SFINAE.
       */
      template <typename T, typename Enable = void>
      struct is_streamable : std::false_type {};

      /**
       * @brief Specialization of is_streamable for streamable types.
       * @tparam T Type to check.
       * @tparam Enable Enable parameter to enable SFINAE.
       */
      template <typename T>
      struct is_streamable<T,
                           std::void_t<decltype(std::declval<std::ostream &>()
                                                << std::declval<T>())>>
          : std::true_type {};

      // Also, C++11 doesn't have folding expressions, so we need to use recursive template specialization
      template <typename... Args> struct all_streamable;

      template <typename First, typename... Rest>
      struct all_streamable<First, Rest...> {
        static constexpr bool value = is_streamable<std::decay_t<First>>::value
                                      && all_streamable<Rest...>::value;
      };

      template <> struct all_streamable<> {
        static constexpr bool value = true;
      };

      /**
       * @brief Stringify a list of arguments.
       * @tparam ...Args Types of the arguments.
       * @param ...args Arguments to be stringified.
       * @return A string containing the arguments.
       * @throw std::invalid_argument If any argument is not streamable.
       */
      template <typename... Args>
      std::string
      stringify(Args &&...args)
      {
        if(sizeof...(args) == 0) return "";

        static_assert(all_streamable<Args...>::value,
                      "All arguments must be streamable");

        std::ostringstream oss;
        (void)std::initializer_list<int>{
          (oss << std::forward<Args>(args), 0)...};
        return oss.str();
      }
    } // namespace Utility
  } // namespace String
} // namespace Lumex

using Lumex::String::Utility::stringify;

#endif // !LUMEX_UTILITIES_HPP
