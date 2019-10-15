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

std::string replace(const std::string &, const std::string &, const std::string &);
std::wstring replaceW(const std::wstring &, const std::wstring &, const std::wstring &);

std::wstring widen(const std::string & src);
std::string narrow(const std::wstring & src);

}
}

#endif