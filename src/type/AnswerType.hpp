#ifndef VEMT_TYPE_ANSWERTYPE_HPP
#define VEMT_TYPE_ANSWERTYPE_HPP

#include "Param.hpp"

namespace vemt {
namespace type {

enum struct AnswerType : int {
	kString,
	kNumber,
	kPicture,
	kJsonFile,
	kJson,
	kRegex
};

class AnswerTypeParam : public Param<AnswerType> {
public:
	class ParseException : public std::invalid_argument {
		using std::invalid_argument::invalid_argument;
	};
	AnswerTypeParam() noexcept;
	AnswerTypeParam(const AnswerType value);
	AnswerTypeParam(const AnswerTypeParam & answer_type_param);
	const int getAsInt() const;
	void setAsInt(const int v);
	std::wstring toDisplayWstring() const;
	virtual bool isAcceptable(const AnswerType & value) const override;
	virtual const std::string toString() const override;
	static AnswerType parseFromString(const std::string & str);
};

}
}

#endif