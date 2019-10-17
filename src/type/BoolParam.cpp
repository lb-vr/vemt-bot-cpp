#include "BoolParam.hpp"

vemt::type::BoolParam::BoolParam() noexcept : Param() {}

vemt::type::BoolParam::BoolParam(const bool value) : Param() {
	this->set(value);
}

vemt::type::BoolParam::BoolParam(const BoolParam & bool_param) : Param() {
	if (bool_param.isSet()) this->set(bool_param);
}

const int vemt::type::BoolParam::getAsInt() const {
	return this->get() ? 1 : 0;
}

void vemt::type::BoolParam::setAsInt(int value) {
	this->set(value ? true : false);
}

bool vemt::type::BoolParam::isAcceptable(const bool & value) const {
	return true;
}

const std::string vemt::type::BoolParam::toString() const {
	return this->get() ? "true" : "false";
}

