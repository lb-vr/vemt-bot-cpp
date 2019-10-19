#include "SubmissionModel.hpp"

vemt::db::SubmissionModel::SubmissionModel(
        const vemt::type::IntParam & id,
        const vemt::type::IntParam & entry_id,
        const vemt::type::StringParam & package_url,
        const vemt::type::IntParam & current_phase,
        const vemt::type::DatetimeParam & created_at,
        const vemt::type::DatetimeParam & updated_at
    ) noexcept:
    id(id),
    entry_id(entry_id),
    package_url(package_url),
    current_phase(current_phase),
    created_at(created_at),
    updated_at(updated_at){
}
vemt::db::SubmissionModel::SubmissionModel(
        const vemt::type::IntParam & entry_id,
        const vemt::type::StringParam & package_url,
        const vemt::type::IntParam & current_phase
    ) noexcept:
    id(),
    entry_id(entry_id),
    package_url(package_url),
    current_phase(current_phase),
    created_at(),
    updated_at(){
}

const vemt::type::IntParam & vemt::db::SubmissionModel::getId() const{
    return this->id;
}
const vemt::type::IntParam & vemt::db::SubmissionModel::getEntryId() const{
    return this->entry_id;
}
const vemt::type::StringParam & vemt::db::SubmissionModel::getPackageUrl() const{
    return this->package_url;
}
const vemt::type::IntParam & vemt::db::SubmissionModel::getCurrentPhase() const{
    return this->current_phase;
}
const vemt::type::DatetimeParam & vemt::db::SubmissionModel::getCreatedAt() const{
    return this->created_at;
}
const vemt::type::DatetimeParam & vemt::db::SubmissionModel::getUpdatedAt() const{
    return this->updated_at;
}

void vemt::db::SubmissionModel::setEntryId(const vemt::type::IntParam & value){
    this->entry_id = value;
}
void vemt::db::SubmissionModel::setPackageUrl(const vemt::type::StringParam & value){
    this->package_url = value;
}
void vemt::db::SubmissionModel::setCurrentPhase(const vemt::type::IntParam & value){
    this->current_phase = value;
}

std::string vemt::db::SubmissionModel::toString(){
    std::stringstream ret;

    std::time_t __created_at = this->getCreatedAt();
    const tm*    _created_at = std::localtime(&__created_at);

    std::time_t __updated_at = this->getUpdatedAt();
    const tm*    _updated_at = std::localtime(&__updated_at);

    ret << "{model: 'SubmissionModel', "
        << "id: "<< this->getId() << ", "
        << "entry_id: " << this->getEntryId() << ", "
        << "package_url: '" << this->getPackageUrl() << "', "
        << "current_phase: '" << this->getCurrentPhase() << "', "
        << "created_at: '" << std::put_time(_created_at, "%F_%T") << "', "
        << "updated_at: '" << std::put_time(_updated_at, "%F_%T") << "'}";
    return ret.str();
}
