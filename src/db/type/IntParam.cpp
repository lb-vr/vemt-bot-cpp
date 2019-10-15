#include "Param.hpp"

bool vemt::db::type::IntParam::isAcceptable(const int & value) const
{
	return true;
}

const std::string vemt::db::type::IntParam::toString() const {
	return std::to_string(this->get());
}