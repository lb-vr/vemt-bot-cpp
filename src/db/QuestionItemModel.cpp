#include "QuestionItemModel.hpp"
#include <sstream>
#include <ctime>

vemt::db::QuestionItemModel::QuestionItemModel(
		const vemt::type::IntParam & id,
		const vemt::type::WstringParam & text,
		const vemt::type::WstringParam & detail_text,
		const vemt::type::AnswerTypeParam & type,
		const vemt::type::WstringParam & regex_rule,
		const std::vector<vemt::type::WstringParam> & choise,
		const vemt::type::IntParam & length,
		const vemt::type::BoolParam & is_required,
		const vemt::type::PhaseParam & required_when_phase,
		const vemt::type::DatetimeParam & required_when_datetime,
		const vemt::type::BoolParam & multiline,
		const vemt::type::DatetimeParam & created_at
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
	const vemt::type::WstringParam & text,
	const vemt::type::WstringParam & detail_text,
	const vemt::type::AnswerTypeParam & type,
	const vemt::type::WstringParam & regex_rule,
	const std::vector<vemt::type::WstringParam> & choise,
	const vemt::type::IntParam & length,
	const vemt::type::BoolParam & is_required,
	const vemt::type::PhaseParam & required_when_phase,
	const vemt::type::DatetimeParam & required_when_datetime,
	const vemt::type::BoolParam & multiline) noexcept
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

const vemt::type::IntParam & vemt::db::QuestionItemModel::getId() const {
	return this->id_;
}
const vemt::type::WstringParam & vemt::db::QuestionItemModel::getText() const {
	return this->text_;
}
const vemt::type::WstringParam & vemt::db::QuestionItemModel::getDetailText() const {
	return this->detail_text_;
}
const vemt::type::AnswerTypeParam & vemt::db::QuestionItemModel::getType() const {
	return this->type_;
}
const vemt::type::WstringParam & vemt::db::QuestionItemModel::getRegexRule() const {
	return this->regex_rule_;
}
const std::vector<vemt::type::WstringParam> & vemt::db::QuestionItemModel::getChoise() const {
	return this->choise_;
}
const vemt::type::IntParam & vemt::db::QuestionItemModel::getLength() const {
	return this->length_;
}
const vemt::type::BoolParam & vemt::db::QuestionItemModel::getIsRequired() const {
	return this->is_required_;
}
const vemt::type::PhaseParam & vemt::db::QuestionItemModel::getRequiredWhenPhase() const {
	return this->required_when_phase_;
}
const vemt::type::DatetimeParam & vemt::db::QuestionItemModel::getRequireWhenDatetime() const {
	return this->required_when_datetime_;
}
const vemt::type::BoolParam & vemt::db::QuestionItemModel::getMultiline() const {
	return this->multiline_;
}
const vemt::type::DatetimeParam & vemt::db::QuestionItemModel::getCreatedAt() const {
	return this->created_at_;
}

void vemt::db::QuestionItemModel::setText(const vemt::type::WstringParam & value){
	this->text_ = value;
}
void vemt::db::QuestionItemModel::setDetailText(const vemt::type::WstringParam & value){
	this->detail_text_ = value;
}
void vemt::db::QuestionItemModel::setType(const vemt::type::AnswerTypeParam & value){
	this->type_ = value;
}
void vemt::db::QuestionItemModel::setRegexRule(const vemt::type::WstringParam & value){
	this->regex_rule_ = value;
}
void vemt::db::QuestionItemModel::setChoise(const std::vector<vemt::type::WstringParam> & value){
	this->choise_ = value;
}
void vemt::db::QuestionItemModel::setLength(const vemt::type::IntParam & value){
	this->length_ = value;
}
void vemt::db::QuestionItemModel::setIsRequired(const vemt::type::BoolParam & value){
	this->is_required_ = value;
}
void vemt::db::QuestionItemModel::setRequiredWhenPhase(const vemt::type::PhaseParam & value){
	this->required_when_phase_ = value;
}
void vemt::db::QuestionItemModel::setRequireWhenDatetime(const vemt::type::DatetimeParam & value){
	this->required_when_datetime_ = value;
}
void vemt::db::QuestionItemModel::setMultiline(const vemt::type::BoolParam & value){
	this->multiline_ = value;
}

std::string vemt::db::QuestionItemModel::toString() const {
    std::stringstream ret;

    ret << "{model: 'QuestionItemModel', "
		<< "id: '" << id_.toString() << "', "
		<< "text: '" << text_.toString() << "', "
		<< "detail_text: '" << detail_text_.toString() << "', "
		<< "type: '" << type_.toString() << "', "
		<< "regex_rule: '" << regex_rule_.toString() << "', "
		<< "choices: ["
		;
	for(auto c : this->getChoise()){
		ret << "'" << c.toString() << "', ";
	}
	ret << "], "
		<< "length: '" << length_.toString() << "', "
		<< "is_required: '" << is_required_.toString() << "', "
		<< "required_when_phase: '" << required_when_phase_.toString() << "', "
		<< "required_when_datetime: '" << required_when_datetime_.toString() << "', "
		<< "multiline: '" << multiline_.toString() << "', "
        << "create_at: '" << created_at_.toString() << "'"
		<< "}";
    return ret.str();
}
