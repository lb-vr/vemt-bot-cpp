#include "ResultModel.hpp"
#include <sstream>

vemt::db::ResultModel::ResultModel(
		const vemt::type::IntParam & id,
		const vemt::type::IntParam & submission_id,
		const vemt::type::IntParam & as_phase,
		const vemt::type::BoolParam & is_passed,
		const vemt::type::StringParam & log_text,
		const vemt::type::DatetimeParam & created_at
	) noexcept :
	id_(id),
	submission_id_(submission_id),
	as_phase_(as_phase),
	is_passed_(is_passed),
	log_text_(log_text),
	created_at_(created_at){
}

vemt::db::ResultModel::ResultModel(
		const vemt::type::IntParam & submission_id,
		const vemt::type::IntParam & as_phase,
		const vemt::type::BoolParam & is_passed,
		const vemt::type::StringParam & log_text
	) noexcept :
	submission_id_(submission_id),
	as_phase_(as_phase),
	is_passed_(is_passed),
	log_text_(log_text){
}

vemt::db::ResultModel::ResultModel(const ResultModel & c) noexcept :
	id_(c.id_),
	submission_id_(c.submission_id_),
	as_phase_(c.as_phase_),
	is_passed_(c.is_passed_),
	log_text_(c.log_text_),
	created_at_(c.created_at_)
{}


const vemt::type::IntParam & vemt::db::ResultModel::getId() const{
	return this->id_;
}
const vemt::type::IntParam & vemt::db::ResultModel::getSubmissionId() const{
	return this->submission_id_;
}
const vemt::type::IntParam & vemt::db::ResultModel::getAsPhase() const{
	return this->as_phase_;
}
const vemt::type::BoolParam & vemt::db::ResultModel::getIsPassed() const{
	return this->is_passed_;
}
const vemt::type::StringParam & vemt::db::ResultModel::getLogText() const{
	return this->log_text_;
}
const vemt::type::DatetimeParam & vemt::db::ResultModel::getCreatedAt() const{
	return this->created_at_;
}

void vemt::db::ResultModel::getSubmissionId(const vemt::type::IntParam & value){
	this->submission_id_ = value;
}
void vemt::db::ResultModel::getAsPhase(const vemt::type::IntParam & value){
	this->as_phase_ = value;
}
void vemt::db::ResultModel::getIsPassed(const vemt::type::BoolParam & value){
	this->is_passed_ = value;
}
void vemt::db::ResultModel::getLogText(const vemt::type::StringParam & value){
	this->log_text_ = value;
}

std::string vemt::db::ResultModel::toString() const {
	std::stringstream ret;
    ret << "{model: 'ResultModel', "
		<< "id: '" << id_.get() << "', "
		<< "submission_id: '" << submission_id_.get() << "', "
		<< "as_phase: '" << as_phase_.get() << "', "
		<< "is_passed: '" << is_passed_.get() << "', "
		<< "log_text: '" << log_text_.get() << "', "
        << "created_at: '" << created_at_.getAsString() << "'"
		<< "}";
    return ret.str();
}
