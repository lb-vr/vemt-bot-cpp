#include "ResultModel.hpp"
#include <sstream>

vemt::db::ResultModel::ResultModel(
		const vemt::db::type::IntParam & id,
		const vemt::db::type::IntParam & submission_id,
		const vemt::db::type::IntParam & as_phase,
		const vemt::db::type::BoolParam & is_passed,
		const vemt::db::type::StringParam & log_text,
		const vemt::db::type::DatetimeParam & created_at
	) noexcept :
	id_(id),
	submission_id_(submission_id),
	as_phase_(as_phase),
	is_passed_(is_passed),
	log_text_(log_text),
	created_at_(created_at){
}

vemt::db::ResultModel::ResultModel(
		const vemt::db::type::IntParam & submission_id,
		const vemt::db::type::IntParam & as_phase,
		const vemt::db::type::BoolParam & is_passed,
		const vemt::db::type::StringParam & log_text
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

const int vemt::db::ResultModel::getId(){return this->id_.get();}
const int vemt::db::ResultModel::getSubmissionId(){return this->submission_id_.get();}
const int vemt::db::ResultModel::getAsPhase(){return this->as_phase_.get();}
const std::string vemt::db::ResultModel::getLogText(){return this->log_text_.get();}
const time_t vemt::db::ResultModel::getCreatedAt(){return this->created_at_.get();}

void vemt::db::ResultModel::setSubmissionId(int v){return this->submission_id_.set(v);}
void vemt::db::ResultModel::setAsPhase(int v){return this->as_phase_.set(v);}
void vemt::db::ResultModel::setLogText(std::string v){return this->log_text_.set(v);}

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
