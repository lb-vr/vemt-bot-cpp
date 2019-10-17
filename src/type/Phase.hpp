#ifndef VEMT_PHASE_HPP
#define VEMT_PHASE_HPP

#include "Param.hpp"

namespace vemt {
namespace type {

enum struct Phase {
	kPreEntry = 1,
	kEntry,
	kSubmit,
	kAccepted
};

class PhaseParam : public Param<Phase> {
public:
	class ParseException : public std::invalid_argument {
		using std::invalid_argument::invalid_argument;
	};
	PhaseParam() noexcept;
	PhaseParam(const Phase value);
	PhaseParam(const PhaseParam & phase_param);

	PhaseParam & operator=(const PhaseParam & phase_param);
	PhaseParam & operator=(const Phase phase);

	int getAsInt(void) const;
	void setAsInt(int int_value);

	std::wstring toDisplayWstring() const noexcept;
	virtual const std::string toString() const override;
	virtual bool isAcceptable(const Phase & value) const override;
	operator int(void) const noexcept;

	static Phase parseFromString(const std::string & str);

};

}
}

#endif