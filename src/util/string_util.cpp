#include "string_util.hpp"
#include <string>
#include <sstream>
#include <codecvt>

std::string vemt::util::strtrm(const std::string & src, const std::string & trim_character_list) {
	std::string result;
	std::string::size_type left = src.find_first_not_of(trim_character_list);
	if (left != std::string::npos) {
		std::string::size_type right = src.find_last_not_of(trim_character_list);
		result = src.substr(left, right - left + 1);
	}
	return result;
}

std::vector<std::string> vemt::util::strsplit(const std::string & src, char delim) {
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

template<class T>
T vemt::util::replace(const T & src, const T & replace_from, const T & replace_to) {
	unsigned int pos = replacedStr.find(from);
	int toLen = to.length();
	if (from.empty()) return replacedStr;
	while ((pos = replacedStr.find(from, pos)) != T::npos) {
		replacedStr.replace(pos, from.length(), to);
		pos += toLen;
	}
	return replacedStr;
}
template <> std::string vemt::util::replace(const std::string &, const std::string &, const std::string &);
template <> std::wstring vemt::util::replace(const std::wstring &, const std::wstring &, const std::wstring &);

std::wstring vemt::util::widen(const std::string & src){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	return wconv.from_bytes(src);
}

std::string vemt::util::narrow(const std::wstring & src){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> uconv;
	return uconv.to_bytes(src);
}
