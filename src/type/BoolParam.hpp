#ifndef VEMT_TYPE_BOOLPARAM_HPP
#define VEMT_TYPE_BOOLPARAM_HPP
#include "Param.hpp"

namespace vemt {
namespace type {

class BoolParam : public Param<bool> {
public:
	BoolParam() noexcept;
	BoolParam(const bool value);
	BoolParam(const BoolParam & bool_param);
	const int getAsInt() const;
	void setAsInt(int value);
	virtual bool isAcceptable(const bool & value) const override;
	virtual const std::string toString() const override;
};

}
}

#endif
