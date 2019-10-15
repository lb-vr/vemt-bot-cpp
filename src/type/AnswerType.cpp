#include "AnswerType.hpp"
#include "Param.hpp"

vemt::type::AnswerTypeParam::AnswerTypeParam() noexcept
{
}

vemt::type::AnswerTypeParam::AnswerTypeParam(const AnswerType value)
{
}

vemt::type::AnswerTypeParam::AnswerTypeParam(const AnswerTypeParam & answer_type_param)
{
}

const int vemt::type::AnswerTypeParam::getAsInt() const
{
	return 0;
}

void vemt::type::AnswerTypeParam::setAsInt(const int v)
{
}

bool vemt::type::AnswerTypeParam::isAcceptable(const AnswerType & value) const
{
	return true;
}

const std::string vemt::type::AnswerTypeParam::toString() const
{
	return std::string();
}

vemt::type::AnswerTypeParam vemt::type::AnswerTypeParam::parseFromString(const std::string & str)
{
	return AnswerTypeParam();
}
