#ifndef VEMT_TYPE_DATETIMEPARAM_HPP
#define VEMT_TYPE_DATETIMEPARAM_HPP

#include "Param.hpp"

namespace vemt {
namespace type {

class DatetimeParam : public Param<time_t> {
public:
	DatetimeParam() noexcept;
	DatetimeParam(const time_t t);
	DatetimeParam(const DatetimeParam & p);
	//DatetimeParam(const std::string & v);

	const int getAsInt() const;
	void setAsInt(const int v);

	const std::string getAsString() const;
	void setAsString(const std::string & v, const std::string & format = "%Y-%m-%d %H:%M:%S");

	virtual bool isAcceptable(const time_t & value) const override;
	virtual const std::string toString() const override;
};

}
}

#endif