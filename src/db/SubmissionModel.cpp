#include "SubmissionModel.hpp"

vemt::db::SubmissionModel::SubmissionModel(
        const vemt::type::IntParam id,
        const vemt::type::IntParam discord_uid,
        const vemt::type::StringParam package_url,
        const vemt::type::IntParam current_phase,
        const vemt::type::DatetimeParam created_at,
        const vemt::type::DatetimeParam updated_at
    ) noexcept:
    id(id),
    discord_uid(discord_uid),
    package_url(package_url),
    current_phase(current_phase),
    created_at(created_at),
    updated_at(updated_at){
}
vemt::db::SubmissionModel::SubmissionModel(
        const vemt::type::IntParam discord_uid,
        const vemt::type::StringParam package_url,
        const vemt::type::IntParam current_phase
    ) noexcept:
    id(),
    discord_uid(discord_uid),
    package_url(package_url),
    current_phase(current_phase),
    created_at(),
    updated_at(){
}

int vemt::db::SubmissionModel::getId(){
    return this->id.get();
}
int vemt::db::SubmissionModel::getDiscordUid(){
    return this->discord_uid.get();
}
std::string vemt::db::SubmissionModel::getPackageUrl(){
    return this->package_url.get();
}
int vemt::db::SubmissionModel::getCurrentPhase(){
    return this->current_phase.get();
}
time_t vemt::db::SubmissionModel::getCreatedAt(){
    return this->created_at.get();
}
time_t vemt::db::SubmissionModel::getUpdatedAt(){
    return this->updated_at.get();
}

std::string vemt::db::SubmissionModel::toString(){
    std::stringstream ret;

    std::time_t __created_at = this->getCreatedAt();
    const tm*    _created_at = std::localtime(&__created_at);

    std::time_t __updated_at = this->getUpdatedAt();
    const tm*    _updated_at = std::localtime(&__updated_at);

    ret << "{model: 'SubmissionModel', "
        << "id: "<< this->getId() << ", "
        << "discord_uid: " << this->getDiscordUid() << ", "
        << "package_url: '" << this->getPackageUrl() << "', "
        << "current_phase: '" << this->getCurrentPhase() << "', "
        << "created_at: '" << std::put_time(_created_at, "%F_%T") << "', "
        << "updated_at: '" << std::put_time(_updated_at, "%F_%T") << "'}";
    return ret.str();
}
