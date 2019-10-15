#include "Param.hpp"

const int vemt::db::type::BoolParam::getAsInt() const {
	return (this->get()) ? 1 : 0;
}

void vemt::db::type::BoolParam::setAsInt(int value) {
	this->set(value ? true : false);
}

bool vemt::db::type::BoolParam::isAcceptable(const bool & value) const
{
	return true;
}

const std::string vemt::db::type::BoolParam::toString() const {
	return this->get() ? "true" : "false";
}

