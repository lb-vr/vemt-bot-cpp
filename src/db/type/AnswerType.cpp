#include "AnswerType.hpp"
#include "Param.hpp"

vemt::db::type::AnswerTypeParam::AnswerTypeParam() noexcept
{
}

vemt::db::type::AnswerTypeParam::AnswerTypeParam(const AnswerType value)
{
}

vemt::db::type::AnswerTypeParam::AnswerTypeParam(const AnswerTypeParam & answer_type_param)
{
}

const int vemt::db::type::AnswerTypeParam::getAsInt() const
{
	return 0;
}

void vemt::db::type::AnswerTypeParam::setAsInt(const int v)
{
}

bool vemt::db::type::AnswerTypeParam::isAcceptable(const AnswerType & value) const
{
	return true;
}

const std::string vemt::db::type::AnswerTypeParam::toString() const
{
	return std::string();
}

vemt::db::type::AnswerTypeParam vemt::db::type::AnswerTypeParam::parseFromString(const std::string & str)
{
	return AnswerTypeParam();
}