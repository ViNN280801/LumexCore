#ifndef LUMEX_SETTINGS_FACTORY_HPP
#define LUMEX_SETTINGS_FACTORY_HPP

#include "lumex/LumexExport.hpp"

#include <memory> ///< For RAII-managed pointers on base interface.

#include "ILumexSettings.hpp"
#include "SupportedConfigExtensions.hpp"

namespace Lumex // NOLINT(modernize-concat-nested-namespaces)
{
  namespace Applied
  {
    namespace Settings
    {
      /**
       * @brief Factory class for creating `ILumexSettings` instances.
       * @details Provides a static method to instantiate settings objects based on the
       *          desired configuration file format (e.g., INI).
       * @note Thread-safe: The factory method is stateless and can be called concurrently.
       */
      class LUMEX_API LumexSettingsFactory
      {
      public:
        /**
         * @brief Creates a settings object for the specified file format.
         * @param ext The configuration file format (e.g., `SupportedConfigExtensions::INI`).
         * @return A `std::unique_ptr` to the new `ILumexSettings` instance.
         * @throws std::invalid_argument If the format is unsupported.
         * @warning The caller assumes ownership of the returned pointer, which means:
         *          - The caller is responsible for the lifetime of the returned object.
         *          - The pointer must not be manually deleted! It is managed by `std::unique_ptr`.
         *          - Ownership is non-shared, see https://en.cppreference.com/w/cpp/memory/unique_ptr.
         */
        static std::unique_ptr<ILumexSettings> create(LumexSettingsExtensions ext);
      };
    } // namespace Settings
  } // namespace Applied
} // namespace Lumex

using LumexSettingsFactory = Lumex::Applied::Settings::LumexSettingsFactory;

#endif // !LUMEX_SETTINGS_FACTORY_HPP
