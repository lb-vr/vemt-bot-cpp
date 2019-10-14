﻿#include "QuestionItemModel.hpp"

vemt::db::QuestionItemModel::QuestionItemModel(
		const vemt::db::type::IntParam & id,
		const vemt::db::type::StringParam & text,
		const vemt::db::type::StringParam & detail_text,
		const vemt::db::type::IntParam type,
		const vemt::db::type::StringParam & regex_rule,
		const std::vector<vemt::db::type::StringParam> choise,
		const vemt::db::type::IntParam & length,
		const vemt::db::type::BoolParam & is_required,
		const vemt::db::type::IntParam & required_when_phase,
		const vemt::db::type::DatetimeParam & required_when_datetime,
		const vemt::db::type::BoolParam & multiline,
		const vemt::db::type::DatetimeParam & created_at
	) noexcept
	:	id_(id),
		text_(text),
		detail_text_(detail_text),
		type_(type),
		regex_rule_(regex_rule),
		choise_(choise),
		length_(length),
		is_required_(is_required),
		required_when_phase_(required_when_phase),
		required_when_datetime_(required_when_datetime),
		multiline_(multiline),
		created_at_(created_at)
{}

vemt::db::QuestionItemModel::QuestionItemModel(
	const vemt::db::type::StringParam & text,
	const vemt::db::type::StringParam & detail_text,
	const vemt::db::type::IntParam type,
	const vemt::db::type::StringParam & regex_rule,
	const std::vector<vemt::db::type::StringParam> & choise,
	const vemt::db::type::IntParam length,
	const vemt::db::type::BoolParam is_required,
	const vemt::db::type::IntParam required_when_phase,
	const vemt::db::type::DatetimeParam required_when_datetime,
	const vemt::db::type::BoolParam multiline) noexcept
	:	id_(0),
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

vemt::db::QuestionItemModel::QuestionItemModel(const QuestionItemModel & c) noexcept
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
		multiline_(c.multiline_),
		created_at_(c.created_at_)
{}

vemt::db::QuestionItemModel::~QuestionItemModel() {}

const int vemt::db::QuestionItemModel::getId() const { return this->id_.get(); }
const std::string vemt::db::QuestionItemModel::getText() const { return this->text_.get(); }
const std::string vemt::db::QuestionItemModel::getDetailText() const { return this->detail_text_.get(); }
const int vemt::db::QuestionItemModel::getType() const { return this->type_.get(); }
const std::string vemt::db::QuestionItemModel::getRegexRule() const { return this->regex_rule_.get(); }
const std::vector<std::string> vemt::db::QuestionItemModel::getChoise() const {
	std::vector<std::string> ret;
	for(const auto& s : this->choise_){
		ret.push_back(s.get());
	}
	return ret;
}
const int vemt::db::QuestionItemModel::getLength() const { return this->length_.get(); }
const bool vemt::db::QuestionItemModel::getIsRequired() const { return this->is_required_.get(); }
const int vemt::db::QuestionItemModel::getRequiredWhenPhase() const { return this->required_when_phase_.get(); }
const std::chrono::system_clock::time_point vemt::db::QuestionItemModel::getRequireWhenDatetime() const { return this->required_when_datetime_.get(); }
const bool vemt::db::QuestionItemModel::getMultiline() const { return this->multiline_.get(); }
const std::chrono::system_clock::time_point vemt::db::QuestionItemModel::getCreatedAt() const {return this->created_at_.get();}

void vemt::db::QuestionItemModel::setText(const std::string & text) { this->text_.set(text); }
void vemt::db::QuestionItemModel::setDetailText(const std::string & detail_text) { this->detail_text_.set(detail_text); }
void vemt::db::QuestionItemModel::setType(const int type) { this->type_.set(type); }
void vemt::db::QuestionItemModel::setRegexRule(const std::string & regex_rule) { this->regex_rule_.set(regex_rule); }
void vemt::db::QuestionItemModel::setChoise(const std::vector<std::string> choise) {
	this->choise_.clear();
	for(const auto s : choise){
		this->choise_.push_back(vemt::db::type::StringParam(s));
	}
}
void vemt::db::QuestionItemModel::setLength(const int length) { this->length_.set(length); }
void vemt::db::QuestionItemModel::setIsRequired(const bool is_required) { this->is_required_.set(is_required); }
void vemt::db::QuestionItemModel::setRequiredWhenPhase(const int phase) { this->required_when_phase_.set(phase); }
void vemt::db::QuestionItemModel::setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint) { this->required_when_datetime_.set(timepoint); }
void vemt::db::QuestionItemModel::setMultiline(const bool multiline) { this->multiline_.set(multiline); }

std::string vemt::db::QuestionItemModel::toString() const {
    std::stringstream ret;

	std::time_t __t0 = std::chrono::system_clock::to_time_t(this->getRequireWhenDatetime());
	const tm*    _t0 = std::localtime(&__t0);

    std::time_t __t1 = std::chrono::system_clock::to_time_t(this->getCreatedAt());
    const tm*    _t1 = std::localtime(&__t1);

    ret << "{model: 'QuestionItemModel', "
		<< "id: '" << id_.get() << "', "
		<< "text: '" << text_.get() << "', "
		<< "detail_text: '" << detail_text_.get() << "', "
		<< "type: '" << type_.get() << "', "
		<< "regex_rule: '" << regex_rule_.get() << "', "
		<< "choices: ["
		;
		for(auto c : this->getChoise()){
			ret << "'" << c << "', ";
		}
	ret << "], "
		<< "length: '" << length_.get() << "', "
		<< "is_required: '" << is_required_.get() << "', "
		<< "required_when_phase: '" << required_when_phase_.get() << "', "
		<< "required_when_datetime: '" << required_when_datetime_.getAsString() << "', "
		<< "multiline: '" << multiline_.get() << "', "
        << "create_at: '" << created_at_.getAsString() << "'"
		<< "}";
    return ret.str();
}
