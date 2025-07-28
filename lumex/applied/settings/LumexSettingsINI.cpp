#define LUMEX_IMPLEMENTATION
#include <array>
#include <fstream>
#include <regex>

#include "lumex/core/filesystem/LumexFilesystem"
#include "lumex/core/filesystem/LumexFilesystem.hpp"

#include "LumexSettingsINI.hpp"

namespace
{
  // util function to trim
  inline std::string
  _trim(std::string const &str)
  {
    auto left = str.find_first_not_of(" \t\r\n");
    if(left == std::string::npos) return "";
    auto right = str.find_last_not_of(" \t\r\n");
    return str.substr(left, right - left + 1);
  }

  // Helper to quote values for saving if they contain special characters or leading/trailing spaces
  inline std::string
  _quote_if_needed(std::string const &value)
  {
    if(value.find_first_of(";#=\"") != std::string::npos
       || (!value.empty() && (value.front() == ' ' || value.back() == ' ')))
    {
      std::string quoted = "\"";
      for(char chr : value)
      {
        if(chr == '"' || chr == '\\') quoted += '\\';
        quoted += chr;
      }
      quoted += "\"";
      return quoted;
    }
    return value;
  }
} // anonymous namespace

LUMEX_PUBLIC_API
bool
LumexSettingsINI::is_ini_valid(std::string const &path)
{
  if(!Lumex::Filesystem::is_readable(path)) return false;

  std::ifstream file(path.c_str());
  if(!file.is_open()) return false;

  // Skip BOM if present
  std::array<char, Constants::UTF8_BOM_SIZE> bom{};
  file.read(bom.data(), Constants::UTF8_BOM_SIZE);
  if(file.gcount() != Constants::UTF8_BOM_SIZE || static_cast<unsigned char>(bom[0]) != Constants::UTF8_BOM_0
     || static_cast<unsigned char>(bom[1]) != Constants::UTF8_BOM_1
     || static_cast<unsigned char>(bom[2]) != Constants::UTF8_BOM_2)
  {
    file.seekg(0);
  }

  std::string line;
  std::regex section_re(Constants::REGEX_SECTION);
  std::regex kv_re(Constants::REGEX_KEY_VALUE);

  while(std::getline(file, line))
  {
    std::string trimmed_line = _trim(line);
    if(trimmed_line.empty() || trimmed_line[0] == ';' || trimmed_line[0] == '#') continue;

    if(std::regex_match(trimmed_line, section_re)) continue;

    // Check for key-value, allowing for inline comments
    size_t comment_pos = trimmed_line.find_first_of(";#");
    if(comment_pos != std::string::npos) trimmed_line = _trim(trimmed_line.substr(0, comment_pos));

    if(std::regex_match(trimmed_line, kv_re)) continue;

    return false; // Line is not a valid comment, section, or key-value pair
  }

  return true;
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::is_ini_valid(char const *path)
{
  if(path == nullptr)
    return false; // FIX(Test: LumexSettingsINITest.GivenNullFilePath_WhenIsIniValid_ThenReturnsFalse): Handle nullptr
                  // input. This line avoids exception when path is nullptr.
  return is_ini_valid(std::string(path));
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::_load_with_parser(std::string const &path)
{
  std::ifstream file(path.c_str());
  if(!file.is_open()) return false;

  // Skip BOM if present
  std::array<char, Constants::UTF8_BOM_SIZE> bom{};
  file.read(bom.data(), Constants::UTF8_BOM_SIZE);
  if(file.gcount() != Constants::UTF8_BOM_SIZE || static_cast<unsigned char>(bom[0]) != Constants::UTF8_BOM_0
     || static_cast<unsigned char>(bom[1]) != Constants::UTF8_BOM_1
     || static_cast<unsigned char>(bom[2]) != Constants::UTF8_BOM_2)
  {
    file.seekg(0);
  }

  m_settings.clear();
  std::string currentSection;
  std::string line;
  while(std::getline(file, line))
  {
    // Handle inline comments first
    size_t comment_pos = line.find_first_of(";#");
    if(comment_pos != std::string::npos) line = line.substr(0, comment_pos);

    std::string trimmed_line = _trim(line);
    if(trimmed_line.empty()) continue;

    if(trimmed_line.front() == '[' && trimmed_line.back() == ']')
    {
      currentSection = _trim(trimmed_line.substr(1, trimmed_line.size() - 2));
      continue;
    }

    auto eqPos = trimmed_line.find('=');
    if(eqPos == std::string::npos) continue;

    std::string key   = _trim(trimmed_line.substr(0, eqPos));
    std::string value = _trim(trimmed_line.substr(eqPos + 1));

    // Handle quoted values
    if(value.size() >= 2 && value.front() == '"' && value.back() == '"')
    {
      value = value.substr(1, value.size() - 2);
      // A more advanced parser would un-escape characters like \\ and \" here
    }

    if(!key.empty()) m_settings[currentSection][key] = value;
  }
  return true; // Return true even if empty, load was successful
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::_save_with_parser(std::string const &path) const
{
  auto parent = Lumex::Path(path).parent_path();
  if(!parent.empty() && !Lumex::Filesystem::exists(parent)) Lumex::Filesystem::create_directories(parent);

  std::ofstream file(path.c_str());
  if(!file.is_open()) return false;

  bool firstSection = true;
  for(auto const &secPair : m_settings)
  {
    if(!firstSection && !secPair.second.empty()) file << "\n";

    if(!secPair.first.empty()) file << "[" << secPair.first << "]\n";

    for(auto const &kvEntry : secPair.second) file << kvEntry.first << "=" << _quote_if_needed(kvEntry.second) << "\n";

    if(!secPair.second.empty()) firstSection = false;
  }
  file.close();
  return file.good();
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::load(std::string const &path)
{
  if(!is_ini_valid(path)) return false;
  return _load_with_parser(path);
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::load(char const *path)
{
  if(path == nullptr)
    return false; // FIX(Test: LumexSettingsINITest.GivenNullFilePath_WhenLoad_ThenReturnsFalse): Handle nullptr
  return load(std::string(path));
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::save(std::string const &path) const
{
  return _save_with_parser(path);
}

LUMEX_PUBLIC_API
bool
LumexSettingsINI::save(char const *path) const
{
  if(path == nullptr)
    return false; // FIX(Test: LumexSettingsINITest.GivenNullFilePath_WhenSave_ThenReturnsFalse): Handle nullptr
  return save(std::string(path));
}

LUMEX_PUBLIC_API
std::string
LumexSettingsINI::get(std::string const &section, std::string const &key) const
{
  if(section.empty() || key.empty() || m_settings.empty()) return "";

  auto sectionIt{m_settings.find(section)};
  if(sectionIt == m_settings.end()) return "";

  auto keyIt{sectionIt->second.find(key)};
  if(keyIt == sectionIt->second.end()) return "";

  return keyIt->second;
}

LUMEX_PUBLIC_API
std::string
LumexSettingsINI::get(char const *section, char const *key) const
{
  if(section == nullptr || key == nullptr)
    return ""; // FIX(Test: LumexSettingsINITest.GivenNullArguments_WhenGet_ThenReturnsEmptyString): Handle nullptr
               // input. This line avoids exception when section or key is nullptr.
  return get(std::string(section), std::string(key));
}

LUMEX_PUBLIC_API
void
LumexSettingsINI::add(std::string const &section, std::string const &key, std::string const &value)
{
  if(section.empty() || key.empty() || value.empty()) return;
  m_settings[section][key] = value;
}

LUMEX_PUBLIC_API
void
LumexSettingsINI::remove(std::string const &section, std::string const &key)
{
  if(section.empty() || key.empty() || m_settings.empty()) return;

  auto sectionIt{m_settings.find(section)};
  if(sectionIt == m_settings.end()) return;

  auto &keyMap{sectionIt->second};
  auto keyIt{keyMap.find(key)};
  if(keyIt == keyMap.end()) return;

  size_t removedCount{keyMap.erase(key)};
  if(removedCount == 0) return;
}

LUMEX_PUBLIC_API
void
LumexSettingsINI::remove(char const *section, char const *key)
{
  if(section == nullptr || key == nullptr)
    return; // FIX(Test: LumexSettingsINITest.GivenNullArguments_WhenRemove_ThenDoesNothing): Handle nullptr
            // input. This line avoids exception when section or key is nullptr.
  remove(std::string(section), std::string(key));
}
