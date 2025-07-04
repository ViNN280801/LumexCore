#define LUMEX_IMPLEMENTATION
#include <stdexcept>

#include "LumexSettingsFactory.hpp"
#include "LumexSettingsINI.hpp"

LUMEX_PUBLIC_API
std::unique_ptr<ILumexSettings>
LumexSettingsFactory::create(LumexSettingsExtensions ext)
{
  switch(ext)
    {
    case LumexSettingsExtensions::INI:
      return std::make_unique<LumexSettingsINI>();
    default:
      throw std::invalid_argument("Unsupported file extension: "
                                  + std::to_string(ext));
    }
}
