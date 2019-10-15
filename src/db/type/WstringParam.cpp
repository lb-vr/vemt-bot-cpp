#include "Param.hpp"
#include "util/string_util.hpp"

void vemt::db::type::WstringParam::setAsCStr(const unsigned char * c_str, size_t len) {
	std::string ret = "";
	for (size_t l = 0; l < len; l++) {
		ret.push_back(c_str[l]);
	}
	this->set(util::widen(ret));
}

bool vemt::db::type::WstringParam::isAcceptable(const std::wstring & value) const
{
	return true;
}

const std::string vemt::db::type::WstringParam::toString() const {
	return util::narrow(this->get());
}
