#ifndef VEMT_BOT_STRING_UTIL_HPP
#define VEMT_BOT_STRING_UTIL_HPP

#include <string>

namespace vemt {
namespace bot {

///
/// @brief Trim string.
/// @param source source string.
/// @param trim_char_list trimming characters.
/// @return trimmed string.
///
std::string strtrm(const std::string & src, const std::string & trim_character_list = " \t\v\r\n");

}
}

#endif