#include "DatetimeParam.hpp"
#include "util/Logger.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdlib>

// vemt::type::DatetimeParam::DatetimeParam(const std::string & v) : Param() 
// { this->setAsString(v); }

vemt::type::DatetimeParam::DatetimeParam() noexcept : Param() {}

vemt::type::DatetimeParam::DatetimeParam(const time_t t) : Param() {
	this->set(t);
}

vemt::type::DatetimeParam::DatetimeParam(const DatetimeParam & p) : Param() {
	if (p.isSet()) this->set(p.get());
}

const int vemt::type::DatetimeParam::getAsInt() const {
	return static_cast<int>(this->get());
	//return std::chrono::duration_cast<std::chrono::seconds>(this->get().time_since_epoch()).count();
}

void vemt::type::DatetimeParam::setAsInt(const int v) {
	this->set(v);
}

const std::string vemt::type::DatetimeParam::getAsString() const {
	std::stringstream ss;
	std::time_t _value = this->get();
	const tm*  __value = std::localtime(&_value);
	ss << std::put_time(__value, "%F %T");
	return ss.str();
}

void vemt::type::DatetimeParam::setAsString(const std::string & v, const std::string & format) {
	std::stringstream ss(v);
	std::tm tm = {};
	ss >> std::get_time(&tm, format.c_str());
	this->set(std::mktime(&tm));
}

bool vemt::type::DatetimeParam::isAcceptable(const time_t & value) const
{
	return true;
}

const std::string vemt::type::DatetimeParam::toString() const {
	return this->getAsString();
}