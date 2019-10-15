#include "string_util.hpp"
#include <string>
#include <sstream>
#include <codecvt>
#include <locale>

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

std::string vemt::util::replace(const std::string & _src, const std::string & replace_from, const std::string & replace_to) {
	std::string src = _src;
	unsigned int pos = src.find(replace_from);
	int toLen = replace_to.length();
	if (replace_from.empty()) return src;
	while ((pos = src.find(replace_from, pos)) != std::string::npos) {
		src.replace(pos, replace_from.length(), replace_to);
		pos += toLen;
	}
	return src;
}

std::wstring vemt::util::replaceW(const std::wstring & _src, const std::wstring & replace_from, const std::wstring & replace_to) {
	std::wstring src = _src;
	unsigned int pos = src.find(replace_from);
	int toLen = replace_to.length();
	if (replace_from.empty()) return src;
	while ((pos = src.find(replace_from, pos)) != std::wstring::npos) {
		src.replace(pos, replace_from.length(), replace_to);
		pos += toLen;
	}
	return src;
}

std::wstring vemt::util::widen(const std::string & src){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wconv;
	return wconv.from_bytes(src);
}

std::string vemt::util::narrow(const std::wstring & src){
	std::wstring_convert<std::codecvt_utf8<wchar_t>> uconv;
	return uconv.to_bytes(src);
}
