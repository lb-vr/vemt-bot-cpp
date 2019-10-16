#include "AnswerType.hpp"
#include "Param.hpp"
#include "util/Logger.hpp"

vemt::type::AnswerTypeParam::AnswerTypeParam() noexcept : Param() {}

vemt::type::AnswerTypeParam::AnswerTypeParam(const AnswerType value) : Param() {
	this->set(value);
}

vemt::type::AnswerTypeParam::AnswerTypeParam(const AnswerTypeParam & answer_type_param) : Param() {
	if (answer_type_param.isSet()) this->set(answer_type_param.get());
}

const int vemt::type::AnswerTypeParam::getAsInt() const {
	return static_cast<int>(this->get());
}

void vemt::type::AnswerTypeParam::setAsInt(const int v) {
	this->set(static_cast<AnswerType>(v));
}

std::wstring vemt::type::AnswerTypeParam::toDisplayWstring() const {
	switch (this->get()) {
	case AnswerType::kString: return L"文字列";
	case AnswerType::kNumber: return L"数値";
	case AnswerType::kPicture: return L"JPG/PNG画像";
	case AnswerType::kJsonFile: return L"Jsonファイル";
	case AnswerType::kJson: return L"Json文字列";
	case AnswerType::kRegex: return L"文字列";
	}
	wAbort("INVALID VALUE.");
	return std::wstring(L"");
}

bool vemt::type::AnswerTypeParam::isAcceptable(const AnswerType & value) const {
	switch (value) {
	case AnswerType::kString:
	case AnswerType::kNumber:
	case AnswerType::kPicture:
	case AnswerType::kJsonFile:
	case AnswerType::kJson:
	case AnswerType::kRegex:
		return true;
	}
	return false;
}

const std::string vemt::type::AnswerTypeParam::toString() const {
	switch (this->get()) {
	case AnswerType::kString: return "string";
	case AnswerType::kNumber: return "number";
	case AnswerType::kPicture: return "picture";
	case AnswerType::kJsonFile: return "jsonfile";
	case AnswerType::kJson: return "json";
	case AnswerType::kRegex: return "regex";
	}
	wAbort("INVALID VALUE.");
	return std::string("");
}

vemt::type::AnswerType vemt::type::AnswerTypeParam::parseFromString(const std::string & str) {
	if (str == "string")		return AnswerType::kString;
	else if (str == "number")	return AnswerType::kNumber;
	else if (str == "picture")	return AnswerType::kPicture;
	else if (str == "jsonfile") return AnswerType::kJsonFile;
	else if (str == "json")		return AnswerType::kJson;
	else if (str == "regex")	return AnswerType::kRegex;
	throw ParseException("Failed to parse " + str + " as AnswerType.");
}
