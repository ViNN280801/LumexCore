#include "lumex/core/base64/Validator.hpp"
#include "lumex/core/base64/Base64.hpp"

using namespace Lumex::Core::Base64;
using namespace Lumex::Core::Base64::Types;

#if __cplusplus >= 201703L
bool
Validator::is_valid_base64(std::string_view str)
#else
bool
Validator::is_valid_base64(std::string const &str)
#endif
{
  if(str.empty() || str.length() % 4 != 0) return false;

  for(size_t i = 0UL; i < str.length(); ++i)
    if(!detail::_is_base64_char_impl(static_cast<byte_type>(str.at(i)))) return false;

  // Check padding
  size_t padding_start = str.find('=');
  if(padding_start != std::string::npos)
  {
    // Padding can only be at the end
    for(size_t i = padding_start; i < str.length(); ++i)
      if(str[i] != '=') return false;

    // At most 2 padding characters
    if(str.length() - padding_start > 2) return false;
  }

  return true;
}
