#include "QuestionItem.hpp"
#include "json11.hpp"
#include "Logger.hpp"
#include "util/string_util.hpp"

vemt::bot::QuestionItem::QuestionItem() noexcept
{
}

vemt::bot::QuestionItem::~QuestionItem() {}

vemt::bot::QuestionItem::ValidationResult vemt::bot::QuestionItem::validate(const std::wstring & answer) const {
	return ValidationResult::kOk;	// TODO: まだ実装していない
}

const int vemt::bot::QuestionItem::getId() const { return this->model_.getId(); }
const std::wstring & vemt::bot::QuestionItem::getText() const { return util::widen(this->model_.getText()); }
const std::wstring & vemt::bot::QuestionItem::getDetailText() const { return util::widen(this->model_.getDetailText()); }
const vemt::bot::AnswerType vemt::bot::QuestionItem::getType() const { return AnswerType(this->model_.getType()); }
const std::wregex vemt::bot::QuestionItem::getRegex() const { return std::wregex(this->getRegexRule()); }
const std::wstring vemt::bot::QuestionItem::getRegexRule() const { return util::widen(this->model_.getRegexRule()); }
const std::vector<std::wstring>& vemt::bot::QuestionItem::getChoise() const { 
	std::vector<std::wstring> ret;
	for (const auto & str : this->model_.getChoise()) ret.push_back(util::widen(str));
	return ret;
}
const unsigned int vemt::bot::QuestionItem::getLength() const { return this->model_.getLength(); }
const bool vemt::bot::QuestionItem::getIsRequired() const { return this->model_.getIsRequired(); }
const vemt::Phase vemt::bot::QuestionItem::getRequiredWhenPhase() const { return Phase(this->model_.getRequiredWhenPhase()); }
const std::chrono::system_clock::time_point vemt::bot::QuestionItem::getRequireWhenDatetime() const { return this->model_.getRequireWhenDatetime(); }
const bool vemt::bot::QuestionItem::getMultiline() const { return this->model_.getMultiline(); }

void vemt::bot::QuestionItem::setText(const std::wstring & text) { this->model_.setText(util::narrow(text)); }
void vemt::bot::QuestionItem::setDetailText(const std::wstring & detail_text) { this->model_.setDetailText(util::narrow(detail_text)); }
void vemt::bot::QuestionItem::setType(const AnswerType type) { this->model_.setType(type.getAsInt()); }
void vemt::bot::QuestionItem::setRegexRule(const std::wstring & regex_rule) { this->model_.setRegexRule(util::narrow(regex_rule)); }
void vemt::bot::QuestionItem::setChoise(const std::vector<std::wstring>& choise) {
	std::vector<std::string> ret;
	for (const auto & str : choise) ret.push_back(util::narrow(str));
	this->model_.setChoise(ret);
}
void vemt::bot::QuestionItem::setLength(const unsigned int length) { this->model_.setLength(length); }
void vemt::bot::QuestionItem::setIsRequired(const bool is_required) { this->model_.setIsRequired(is_required); }
void vemt::bot::QuestionItem::setRequiredWhenPhase(const vemt::Phase phase) { this->model_.setRequiredWhenPhase(phase); }
void vemt::bot::QuestionItem::setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint) { this->model_.setRequiredWhenDatetime(timepoint); }
void vemt::bot::QuestionItem::setMultiline(const bool multiline) { this->setMultiline(multiline); }
std::string vemt::bot::QuestionItem::toString() const {
	return std::string("QuestionItem instance.");
}

vemt::bot::QuestionItem vemt::bot::QuestionItem::createFromJson(const json11::Json & json, std::string & error_msg) {
	QuestionItem ret;
	ret.setText(util::replace(json["text"].asWString(L""), std::wstring(L"\n"), std::wstring(L"")));

	// detail_text
	{
		auto detail_str = std::wstring();
		if (json["details"].is_array()) {
			for (const auto & detail : json["details"].arrayItems()) {
				auto line = detail.asWString(L"");
				if (!line.empty()) detail_str += (util::replace(line, std::wstring(L"\n"), std::wstring(L"")) + L"\\n");
			}
		}
		ret.setDetailText(detail_str);
	}

	// type
	try {
		ret.setType(AnswerType::parseFromString(json["type"].asString("string")));
	}
	catch (const AnswerType::ParseError & e) {
		throw JsonParseError(L"typeの値が不正です。詳しくは`+config help question`を参照してください。");
	}
	
	// regex
	ret.setRegexRule(json["regex_rule"].asWString(L".+"));

	// choise
	auto choise = std::vector<std::wstring>();
	if (json["choise"].is_array()) {
		for (const auto & choise_json : json["choise"].arrayItems()) {
			auto item = choise_json.asWString(L"");
			if (!item.empty()) choise.push_back(util::replace(item, std::wstring(L"\n"), std::wstring(L"")));
		}
	}

	// length
	unsigned int length = json["length"].asInt(128);

	// required
	auto is_required = json["is_required"].asBool(false);
	auto required_when_phase = json["required_when_phase"].asInt(Phase::kPublish.to_int());
	auto required_when_datetime = std::chrono::system_clock::now();	// TODO: jsonから指定

	// multiline
	auto multiline = json["multiline"].asBool(false);
		
	return ret;
}

vemt::bot::QuestionItem::JsonParseError::JsonParseError(const std::wstring & str) noexcept
	: std::invalid_argument(util::narrow(str).c_str()) {
	logging::warn << "Failed to parse. Message = " << util::narrow(str) << std::endl;
}