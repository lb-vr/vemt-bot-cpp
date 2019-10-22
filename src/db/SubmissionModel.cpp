#include "SubmissionModel.hpp"

vemt::db::SubmissionModel::SubmissionModel(
        const vemt::type::IntParam & id,
        const vemt::type::IntParam & entry_id,
        const vemt::type::StringParam & package_url,
        const vemt::type::PhaseParam & current_phase,
        const vemt::type::DatetimeParam & created_at,
        const vemt::type::DatetimeParam & updated_at
    ) noexcept:
    id_(id),
    entry_id_(entry_id),
    package_url_(package_url),
    current_phase_(current_phase),
    created_at_(created_at),
    updated_at_(updated_at){
}
vemt::db::SubmissionModel::SubmissionModel(
        const vemt::type::IntParam & entry_id,
        const vemt::type::StringParam & package_url,
        const vemt::type::PhaseParam & current_phase
    ) noexcept:
    id_(),
    entry_id_(entry_id),
    package_url_(package_url),
    current_phase_(current_phase),
    created_at_(),
    updated_at_(){
}

const vemt::type::IntParam & vemt::db::SubmissionModel::getId() const{
    return this->id_;
}
const vemt::type::IntParam & vemt::db::SubmissionModel::getEntryId() const{
    return this->entry_id_;
}
const vemt::type::StringParam & vemt::db::SubmissionModel::getPackageUrl() const{
    return this->package_url_;
}
const vemt::type::PhaseParam & vemt::db::SubmissionModel::getCurrentPhase() const{
    return this->current_phase_;
}
const vemt::type::DatetimeParam & vemt::db::SubmissionModel::getCreatedAt() const{
    return this->created_at_;
}
const vemt::type::DatetimeParam & vemt::db::SubmissionModel::getUpdatedAt() const{
    return this->updated_at_;
}

void vemt::db::SubmissionModel::setEntryId(const vemt::type::IntParam & value){
    this->entry_id_ = value;
}
void vemt::db::SubmissionModel::setPackageUrl(const vemt::type::StringParam & value){
    this->package_url_ = value;
}
void vemt::db::SubmissionModel::setCurrentPhase(const vemt::type::PhaseParam & value){
    this->current_phase_ = value;
}

std::string vemt::db::SubmissionModel::toString(){
    std::stringstream ret;

    std::time_t __created_at = this->getCreatedAt();
    const tm*    _created_at = std::localtime(&__created_at);

    std::time_t __updated_at = this->getUpdatedAt();
    const tm*    _updated_at = std::localtime(&__updated_at);

    ret << "{model: 'SubmissionModel', "
        << "id: "<< this->id_.toString() << ", "
        << "entry_id: " << this->entry_id_.toString() << ", "
        << "package_url: '" << this->package_url_.toString() << "', "
        << "current_phase: '" << this->current_phase_.toString() << "', "
        << "created_at: '" << this->created_at_.toString() << "', "
        << "updated_at: '" << this->updated_at_.toString() << "'}";
    return ret.str();
}
