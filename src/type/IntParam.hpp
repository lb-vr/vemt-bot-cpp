#ifndef VEMT_TYPE_INTPARAM_HPP
#define VEMT_TYPE_INTPARAM_HPP

#include "Param.hpp"

namespace vemt {
namespace type {

class IntParam : public Param<int64_t> {
public:
	IntParam() noexcept;
	IntParam(const int64_t value);
	IntParam(const IntParam & int_param);
	virtual bool isAcceptable(const int64_t & value) const override;
	virtual const std::string toString() const override;
};

}
}

#endif