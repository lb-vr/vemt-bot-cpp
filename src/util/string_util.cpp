#include "string_util.hpp"
#include <string>
#include <sstream>
#include <codecvt>

std::string vemt::bot::strtrm(const std::string & src, const std::string & trim_character_list) {
	std::string result;
	std::string::size_type left = src.find_first_not_of(trim_character_list);
	if (left != std::string::npos) {
		std::string::size_type right = src.find_last_not_of(trim_character_list);
		result = src.substr(left, right - left + 1);
	}
	return result;
}

std::vector<std::string> vemt::bot::strsplit(const std::string & src, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(src);
	std::string item;
	while (getline(ss, item, delim)) {
		if (!item.empty()) {
			elems.push_back(item);
		}
	}
	return elems;
}

std::wstring vemt::bot::widen(const std::string & src){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	return wconv.from_bytes(src);
}

std::string vemt::bot::narrow(const std::wstring & src){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> uconv;
	return uconv.to_bytes(src);
}
