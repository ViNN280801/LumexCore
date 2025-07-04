#ifndef LUMEX_TIME_HPP
#define LUMEX_TIME_HPP

#include "lumex/LumexExport.hpp"
#include "lumex/core/utility/LumexUtility"

#include <string>

LUMEX_EXTERN_C_BEGIN

namespace Lumex
{
  namespace Core
  {
    namespace Time
    {
      namespace Constants
      {
        constexpr unsigned long KDEFAULT_DATETIME_BUF_SIZE = 128UL;
        constexpr int K_MICROSECONDS_IN_MILLISECOND        = 1000;
        constexpr long long NS_IN_MCS                      = 1000LL;
        constexpr long long NS_IN_MS  = 1000LL * NS_IN_MCS;
        constexpr long long NS_IN_S   = 1000LL * NS_IN_MS;
        constexpr long long NS_IN_MIN = 60LL * NS_IN_S;
        constexpr long long NS_IN_H   = 60LL * NS_IN_MIN;
        constexpr long long NS_IN_D   = 24LL * NS_IN_H;
        constexpr long long NS_IN_W   = 7LL * NS_IN_D;
        constexpr long long NS_IN_M   = 30LL * NS_IN_D;  // Approximation
        constexpr long long NS_IN_Y   = 365LL * NS_IN_D; // Approximation
      } // namespace Constants

      class LUMEX_API LumexTime
      {
      public:
        /**
         * @brief Get the current date and time in the specified format.
         * @param format The format of the date and time.
         *               Default is Russian format: "%d.%m.%Y_%H:%M:%S".
         * @return The current date and time in the specified format.
         */
        static std::string
        get_current_datetime(char const *format = "%d.%m.%Y_%H:%M:%S");

        /// @brief Gettters for the time since epoch in different time units.
        static std::string get_timestamp_ns();  // nanoseconds
        static std::string get_timestamp_mcs(); // microseconds
        static std::string get_timestamp_ms();  // milliseconds
        static std::string get_timestamp_s();   // seconds
        static std::string get_timestamp_min(); // minutes
        static std::string get_timestamp_h();   // hours
        static std::string get_timestamp_d();   // days
        static std::string get_timestamp_w();   // weeks
        static std::string get_timestamp_m();   // months
        static std::string get_timestamp_y();   // years

      private:
        static std::string _get_timestamp(long long divisor);
      };
    } // namespace Time
  } // namespace Core
} // namespace Lumex

using LumexTime = Lumex::Core::Time::LumexTime;

LUMEX_EXTERN_C_END

#endif // !LUMEX_TIME_HPP
