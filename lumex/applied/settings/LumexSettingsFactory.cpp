#define LUMEX_IMPLEMENTATION
#include "LumexSettingsFactory.hpp"
#include "LumexSettingsINI.hpp"

LUMEX_PUBLIC_API
std::unique_ptr<ILumexSettings>
LumexSettingsFactory::create(LumexSettingsExtensions ext)
{
  switch(ext)
  {
  case LumexSettingsExtensions::INI: return std::unique_ptr<LumexSettingsINI>(new LumexSettingsINI());
  default: return nullptr;
  }
}
