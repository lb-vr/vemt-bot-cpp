#ifndef VEMT_BOT_STRING_UTIL_HPP
#define VEMT_BOT_STRING_UTIL_HPP

#include <string>
#include <vector>

namespace vemt {
namespace util {

///
/// @brief Trim string.
/// @param source source string.
/// @param trim_char_list trimming characters.
/// @return trimmed string.
///
std::string strtrm(const std::string & src, const std::string & trim_character_list = " \t\v\r\n");
std::vector<std::string> strsplit(const std::string & src, char delim);
template <class T> T replace(const T & src, const T & replace_from, const T & replace_to);

std::wstring widen(const std::string & src);
std::string narrow(const std::wstring & src);

}
}

#endif