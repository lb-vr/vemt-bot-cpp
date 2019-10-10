#include "QuestionItemModel.hpp"
#include "json11.hpp"
#include "Logger.hpp"

vemt::bot::QuestionItemModel::QuestionItemModel(
	const unsigned long int id,
	const std::string & text,
	const std::string & detail_text,
	const Type type,
	const std::string & regex_rule,
	const std::vector<std::string>& choise,
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

vemt::bot::QuestionItemModel::QuestionItemModel(
	const std::string & text,
	const std::string & detail_text,
	const Type type,
	const std::string & regex_rule,
	const std::vector<std::string>& choise,
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

vemt::bot::QuestionItemModel::QuestionItemModel(const QuestionItemModel & c) noexcept
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

vemt::bot::QuestionItemModel::~QuestionItemModel() {}

vemt::bot::QuestionItemModel::ValidationResult vemt::bot::QuestionItemModel::validate(const std::string & answer) const {
	return ValidationResult::kOk;	// TODO: まだ実装していない
}

const unsigned long int vemt::bot::QuestionItemModel::getId() const { return this->id_; }
const std::string & vemt::bot::QuestionItemModel::getText() const { return this->text_; }
const std::string & vemt::bot::QuestionItemModel::getDetailText() const { return this->detail_text_; }
const vemt::bot::QuestionItemModel::Type vemt::bot::QuestionItemModel::getType() const { return this->type_; }
const std::regex vemt::bot::QuestionItemModel::getRegex() const { return std::regex(this->getRegexRule()); }
const std::string vemt::bot::QuestionItemModel::getRegexRule() const { return this->regex_rule_; }
const std::vector<std::string>& vemt::bot::QuestionItemModel::getChoise() const { return this->choise_; }
const unsigned int vemt::bot::QuestionItemModel::getLength() const { return this->length_; }
const bool vemt::bot::QuestionItemModel::getIsRequired() const { return this->is_required_; }
const vemt::Phase vemt::bot::QuestionItemModel::getRequiredWhenPhase() const { return this->required_when_phase_; }
const std::chrono::system_clock::time_point vemt::bot::QuestionItemModel::getRequireWhenDatetime() const { return this->required_when_datetime_; }
const bool vemt::bot::QuestionItemModel::getMultiline() const { return this->multiline_; }

void vemt::bot::QuestionItemModel::setText(const std::string & text) { this->text_ = text; }
void vemt::bot::QuestionItemModel::setDetailText(const std::string & detail_text) { this->detail_text_ = detail_text; }
void vemt::bot::QuestionItemModel::setType(const Type type) { this->type_ = type; }
void vemt::bot::QuestionItemModel::setRegexRule(const std::string & regex_rule) { this->regex_rule_ = regex_rule_; }
void vemt::bot::QuestionItemModel::setChoise(const std::vector<std::string>& choise) { this->choise_ = choise; }
void vemt::bot::QuestionItemModel::setLength(const unsigned int length) { this->length_ = length; }
void vemt::bot::QuestionItemModel::setIsRequired(const bool is_required) { this->is_required_ = is_required; }
void vemt::bot::QuestionItemModel::setRequiredWhenPhase(const vemt::Phase phase) { this->required_when_phase_ = phase; }
void vemt::bot::QuestionItemModel::setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint) { this->required_when_datetime_ = timepoint; }
void vemt::bot::QuestionItemModel::setMultiline(const bool multiline) { this->multiline_ = multiline; }

std::string vemt::bot::QuestionItemModel::toString() const {
	return std::string("QuestionItemModel instance.");
}

vemt::bot::QuestionItemModel vemt::bot::QuestionItemModel::createFromJson(const json11::Json & json, std::string & error_msg) {
	// text
	auto text = json["text"].asString("");
	
	// detail_text
	auto detail_str = std::string();
	if (json["details"].is_array()) {
		for (const auto & detail : json["details"].arrayItems()) {
			auto line = detail.asString("");
			if (!line.empty()) detail_str += (line + "\n");
		}
	}
	
	// type
	auto type = QuestionItemModel::str2type(json["type"].asString("string"));
	
	// regex
	std::string regex_rule = json["regex_rule"].asString(".+");

	// choise
	auto choise = std::vector<std::string>();
	if (json["choise"].is_array()) {
		for (const auto & choise_json : json["choise"].arrayItems()) {
			auto item = choise_json.asString("");
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
	
	return QuestionItemModel(text, detail_str, type, regex_rule, choise, length, is_required, required_when_phase, required_when_datetime, multiline);
}

std::string vemt::bot::QuestionItemModel::type2str(const Type type) {
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

vemt::bot::QuestionItemModel::Type vemt::bot::QuestionItemModel::str2type(const std::string & str) {
	if (str == "string") return kString;
	if (str == "number") return kNumber;
	if (str == "picture") return kPicture;
	if (str == "jsonfile") return kJsonFile;
	if (str == "json") return kJson;
	if (str == "regex") return kRegex;
	return kUnknown;
}
