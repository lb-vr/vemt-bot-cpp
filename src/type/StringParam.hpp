#ifndef VEMT_TYPE_STRINGPARAM_HPP
#define VEMT_TYPE_STRINGPARAM_HPP

#include "Param.hpp"

namespace vemt {
namespace type {

class StringParam : public Param<std::string> {
public:
	StringParam() noexcept;
	StringParam(const std::string & value);
	StringParam(const StringParam & string_param);
	//StringParam(const unsigned char *c_str, size_t len);
	void setAsCStr(const unsigned char *c_str, size_t len);
	virtual bool isAcceptable(const std::string & value) const override;
	virtual const std::string toString() const override;
};

}
}

#endif