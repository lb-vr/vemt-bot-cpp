#include "Param.hpp"

//vemt::db::type::StringParam::StringParam(const unsigned char * c_str, size_t len) : Param()
//{ this->setAsCStr(c_str, len); }

void vemt::db::type::StringParam::setAsCStr(const unsigned char * c_str, size_t len) {
	std::string ret = "";
	for (size_t l = 0; l < len; l++) {
		ret.push_back(c_str[l]);
	}
	this->set(ret);
}

bool vemt::db::type::StringParam::isAcceptable(const std::string & value) const
{
	return true;
}

const std::string vemt::db::type::StringParam::toString() const {
	return this->get();
}