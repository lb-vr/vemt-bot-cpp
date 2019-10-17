#include "Param.hpp"

//vemt::type::StringParam::StringParam(const unsigned char * c_str, size_t len) : Param()
//{ this->setAsCStr(c_str, len); }

vemt::type::StringParam::StringParam() noexcept : Param() {}

vemt::type::StringParam::StringParam(const std::string & value) : Param() {
	this->set(value);
}

vemt::type::StringParam::StringParam(const StringParam & string_param) : Param() {
	if (string_param.isSet()) this->set(string_param.get());
}

void vemt::type::StringParam::setAsCStr(const unsigned char * c_str, size_t len) {
	std::string ret = "";
	for (size_t l = 0; l < len; l++) {
		ret.push_back(c_str[l]);
	}
	this->set(ret);
}

bool vemt::type::StringParam::isAcceptable(const std::string & value) const
{
	return true;
}

const std::string vemt::type::StringParam::toString() const {
	return this->get();
}