#include "QuestionItem.hpp"
#include "json11.hpp"
#include "Logger.hpp"

vemt::bot::QuestionItem::QuestionItem(
	const unsigned long int id,
	const std::wstring & text,
	const std::wstring & detail_text,
	const Type type,
	const std::wstring & regex_rule,
	const std::vector<std::wstring>& choise,
	const unsigned int length,
	const bool is_required,
	const Phase required_when_phase,
	const std::chrono::system_clock::time_point required_when_datetime,
	const bool multiline) noexcept
	: id_(id),
		text_(text),
		detail_text_(detail_text),
		type_(type),
		regex_rule_(regex_rule),
		choise_(choise),
		length_(length),
		is_required_(is_required),
		required_when_datetime_(required_when_datetime),
		required_when_phase_(required_when_phase),
		multiline_(multiline)
{}

vemt::bot::QuestionItem::QuestionItem(
	const std::wstring & text,
	const std::wstring & detail_text,
	const Type type,
	const std::wstring & regex_rule,
	const std::vector<std::wstring>& choise,
	const unsigned int length,
	const bool is_required,
	const Phase required_when_phase,
	const std::chrono::system_clock::time_point required_when_datetime,
	const bool multiline) noexcept
	: id_(0),
		text_(text),
		detail_text_(detail_text),
		type_(type),
		regex_rule_(regex_rule),
		choise_(choise),
		length_(length),
		is_required_(is_required),
		required_when_datetime_(required_when_datetime),
		required_when_phase_(required_when_phase),
		multiline_(multiline)
{}

vemt::bot::QuestionItem::QuestionItem(const QuestionItem & c) noexcept
	: id_(c.id_),
		text_(c.text_),
		detail_text_(c.detail_text_),
		type_(c.type_),
		regex_rule_(c.regex_rule_),
		choise_(c.choise_),
		length_(c.length_),
		is_required_(c.is_required_),
		required_when_datetime_(c.required_when_datetime_),
		required_when_phase_(c.required_when_phase_),
		multiline_(c.multiline_)
{}

vemt::bot::QuestionItem::~QuestionItem() {}

vemt::bot::QuestionItem::ValidationResult vemt::bot::QuestionItem::validate(const std::wstring & answer) const {
	return ValidationResult::kOk;	// TODO: まだ実装していない
}

const unsigned long int vemt::bot::QuestionItem::getId() const { return this->id_; }
const std::wstring & vemt::bot::QuestionItem::getText() const { return this->text_; }
const std::wstring & vemt::bot::QuestionItem::getDetailText() const { return this->detail_text_; }
const vemt::bot::QuestionItem::Type vemt::bot::QuestionItem::getType() const { return this->type_; }
const std::wregex vemt::bot::QuestionItem::getRegex() const { return std::wregex(this->getRegexRule()); }
const std::wstring vemt::bot::QuestionItem::getRegexRule() const { return this->regex_rule_; }
const std::vector<std::wstring>& vemt::bot::QuestionItem::getChoise() const { return this->choise_; }
const unsigned int vemt::bot::QuestionItem::getLength() const { return this->length_; }
const bool vemt::bot::QuestionItem::getIsRequired() const { return this->is_required_; }
const vemt::Phase vemt::bot::QuestionItem::getRequiredWhenPhase() const { return this->required_when_phase_; }
const std::chrono::system_clock::time_point vemt::bot::QuestionItem::getRequireWhenDatetime() const { return this->required_when_datetime_; }
const bool vemt::bot::QuestionItem::getMultiline() const { return this->multiline_; }

void vemt::bot::QuestionItem::setText(const std::wstring & text) { this->text_ = text; }
void vemt::bot::QuestionItem::setDetailText(const std::wstring & detail_text) { this->detail_text_ = detail_text; }
void vemt::bot::QuestionItem::setType(const Type type) { this->type_ = type; }
void vemt::bot::QuestionItem::setRegexRule(const std::wstring & regex_rule) { this->regex_rule_ = regex_rule_; }
void vemt::bot::QuestionItem::setChoise(const std::vector<std::wstring>& choise) { this->choise_ = choise; }
void vemt::bot::QuestionItem::setLength(const unsigned int length) { this->length_ = length; }
void vemt::bot::QuestionItem::setIsRequired(const bool is_required) { this->is_required_ = is_required; }
void vemt::bot::QuestionItem::setRequiredWhenPhase(const vemt::Phase phase) { this->required_when_phase_ = phase; }
void vemt::bot::QuestionItem::setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint) { this->required_when_datetime_ = timepoint; }
void vemt::bot::QuestionItem::setMultiline(const bool multiline) { this->multiline_ = multiline; }

std::string vemt::bot::QuestionItem::toString() const {
	return std::string("QuestionItem instance.");
}

vemt::bot::QuestionItem vemt::bot::QuestionItem::createFromJson(const json11::Json & json, std::string & error_msg) {
	// text
	auto text = json["text"].asWString(L"");
	
	// detail_text
	auto detail_str = std::wstring();
	if (json["details"].is_array()) {
		for (const auto & detail : json["details"].arrayItems()) {
			auto line = detail.asWString(L"");
			if (!line.empty()) detail_str += (line + L"\n");
		}
	}
	
	// type
	auto type = QuestionItem::str2type(json["type"].asString("string"));
	
	// regex
	std::wstring regex_rule = json["regex_rule"].asWString(L".+");

	// choise
	auto choise = std::vector<std::wstring>();
	if (json["choise"].is_array()) {
		for (const auto & choise_json : json["choise"].arrayItems()) {
			auto item = choise_json.asWString(L"");
			if (!item.empty()) choise.push_back(item);
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

	// validating
	error_msg = "";
	if (text.empty()) error_msg = "Question text is empty. ";
	if (type == Type::kUnknown) error_msg += "Invalid answer type. ";
	if (regex_rule.empty()) error_msg += "Regex rule is empty. ";
	if (length <= 0 || length > 2048) error_msg += "Invalid length limit. ";
	
	return QuestionItem(text, detail_str, type, regex_rule, choise, length, is_required, required_when_phase, required_when_datetime, multiline);
}

std::string vemt::bot::QuestionItem::type2str(const Type type) {
	switch (type) {
	case kString: return "string";
	case kNumber: return "number";
	case kPicture: return "picture";
	case kJsonFile: return "jsonfile";
	case kJson: return "json";
	case kRegex: return "regex";
	}
	return std::string();
}

vemt::bot::QuestionItem::Type vemt::bot::QuestionItem::str2type(const std::string & str) {
	if (str == "string") return kString;
	if (str == "number") return kNumber;
	if (str == "picture") return kPicture;
	if (str == "jsonfile") return kJsonFile;
	if (str == "json") return kJson;
	if (str == "regex") return kRegex;
	return kUnknown;
}
