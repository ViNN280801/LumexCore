#ifndef ILUMEX_SETTINGS_HPP
#define ILUMEX_SETTINGS_HPP

#include "lumex/LumexExport.hpp"

#include <string>

namespace Lumex
{
  namespace Applied
  {
    namespace Settings
    {
      /**
       * @brief Factory interface for creating and managing settings objects.
       * @details This interface defines the contract for loading, saving, and manipulating
       *          application settings stored in configuration files. Implementations should
       *          handle platform-specific file operations and parsing logic.
       * @note Thread-safety: Implementations must ensure thread-safe access if used in a
       *       multi-threaded context.
       *       Exception safety: Implementations must ensure that no exceptions are thrown
       *       from any of the public member functions.
       */
      class LUMEX_API ILumexSettings
      {
      public:
        /// @brief Defaulted virtual destructor for polymorphic behavior.
        virtual ~ILumexSettings() = default;

        /**
         * @brief Load settings from a configuration file.
         * @details "Load" means that the settings have been successfully read from the file
         *          to the internal storage and can be retrieved by the `get` method
         *          or modified by the `set` method.
         * @param path The filesystem path to the configuration file.
         * @return `true` if the settings were loaded successfully, `false` otherwise.
         * @note The file format (e.g., JSON, INI) is implementation-defined.
         */
        virtual bool load(std::string const &path) = 0;

        /**
         * @brief Save settings to a configuration file.
         * @details "Save" means that the settings have been successfully written to the file
         *          from the internal storage.
         * @param path The filesystem path to the target configuration file.
         * @return `true` if the settings were saved successfully, `false` otherwise.
         */
        virtual bool save(std::string const &path) const = 0;

        /**
         * @brief Retrieve a setting value by section and key.
         * @param section The section name in the configuration file.
         * @param key The key name within the section.
         * @return The value associated with the key, or an empty string if not found.
         * @note Keys and sections are case-sensitive.
         */
        virtual std::string
        get(std::string const &section, std::string const &key) const
          = 0;

        /**
         * @brief Set or update a setting value.
         * @param section The section name in the configuration file.
         * @param key The key name within the section.
         * @param value The new value to assign.
         * @note Do nothing if the section or key does not exist.
         */
        virtual void add(std::string const &section, std::string const &key,
                         std::string const &value)
          = 0;

        /**
         * @brief Remove a setting by section and key.
         * @param section The section name in the configuration file.
         * @param key The key name within the section.
         * @note Do nothing if the section or key does not exist.
         */
        virtual void remove(std::string const &section, std::string const &key)
          = 0;
      };
    } // namespace Settings
  } // namespace Applied
} // namespace Lumex

using ILumexSettings = Lumex::Applied::Settings::ILumexSettings;

#endif // !ILUMEX_SETTINGS_HPP
