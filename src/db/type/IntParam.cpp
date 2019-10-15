#include "Param.hpp"

vemt::db::type::IntParam::IntParam() noexcept : Param() {}

vemt::db::type::IntParam::IntParam(const int value) : Param() {
	this->set(value);
}

vemt::db::type::IntParam::IntParam(const IntParam & int_param) : Param() {
	if (int_param.isSet()) this->set(int_param.get());
}

bool vemt::db::type::IntParam::isAcceptable(const int & value) const {
	return true;
}

const std::string vemt::db::type::IntParam::toString() const {
	return std::to_string(this->get());
}