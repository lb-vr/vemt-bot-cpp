#include "Param.hpp"
#include "util/string_util.hpp"

vemt::type::WstringParam::WstringParam() noexcept : Param() {}

vemt::type::WstringParam::WstringParam(const std::wstring & value) : Param() {
	this->set(value);
}

vemt::type::WstringParam::WstringParam(const WstringParam & wstring_param) : Param() {
	if (wstring_param.isSet()) this->set(wstring_param.get());
}

void vemt::type::WstringParam::setAsCStr(const unsigned char * c_str, size_t len) {
	std::string ret = "";
	for (size_t l = 0; l < len; l++) {
		ret.push_back(c_str[l]);
	}
	this->set(util::widen(ret));
}

bool vemt::type::WstringParam::isAcceptable(const std::wstring & value) const
{
	return true;
}

const std::string vemt::type::WstringParam::toString() const {
	return util::narrow(this->get());
}
