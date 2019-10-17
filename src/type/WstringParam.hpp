#ifndef VEMT_TYPE_WSTRINGPARAM_HPP
#define VEMT_TYPE_WSTRINGPARAM_HPP

#include "Param.hpp"

namespace vemt {
namespace type {

class WstringParam : public Param<std::wstring> {
public:
	WstringParam() noexcept;
	WstringParam(const std::wstring & value);
	WstringParam(const WstringParam & wstring_param);
	void setAsCStr(const unsigned char *c_str, size_t len);
	virtual bool isAcceptable(const std::wstring & value) const override;
	virtual const std::string toString() const override;
};

}
}

#endif