#include "string_util.hpp"

std::string vemt::bot::strtrm(const std::string & src, const std::string & trim_character_list) {
	std::string result;
	std::string::size_type left = src.find_first_not_of(trim_character_list);
	if (left != std::string::npos) {
		std::string::size_type right = src.find_last_not_of(trim_character_list);
		result = src.substr(left, right - left + 1);
	}
	return result;
}