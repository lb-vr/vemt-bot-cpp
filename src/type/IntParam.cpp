#include "IntParam.hpp"

vemt::type::IntParam::IntParam() noexcept : Param() {}

vemt::type::IntParam::IntParam(const int64_t value) : Param() {
	this->set(value);
}

vemt::type::IntParam::IntParam(const IntParam & int_param) : Param() {
	if (int_param.isSet()) this->set(int_param.get());
}

bool vemt::type::IntParam::isAcceptable(const int64_t & value) const {
	return true;
}

const std::string vemt::type::IntParam::toString() const {
	return std::to_string(this->get());
}