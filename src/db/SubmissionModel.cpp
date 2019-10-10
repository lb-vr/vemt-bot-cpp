#include "SubmissionModel.hpp"

vemt::db::SubmissionModel::SubmissionModel(
        const long int id,
        const long int discord_uid,
        const std::string package_url,
        const vemt::Phase current_phase,
        const std::chrono::system_clock::time_point created_at,
        const std::chrono::system_clock::time_point updated_at) noexcept : 
    id(id),
    discord_uid(discord_uid),
    package_url(package_url),
    current_phase(current_phase),
    created_at(created_at),
    updated_at(updated_at){
}

long int vemt::db::SubmissionModel::getId(){
    return this->id;
}
long int vemt::db::SubmissionModel::getDiscordUid(){
    return this->discord_uid;
}
std::string vemt::db::SubmissionModel::getPackageUrl(){
    return this->package_url;
}
vemt::Phase vemt::db::SubmissionModel::getCurrentPhase(){
    return this->current_phase;
}
std::chrono::system_clock::time_point vemt::db::SubmissionModel::getCreatedAt(){
    return this->created_at;
}
std::chrono::system_clock::time_point vemt::db::SubmissionModel::getUpdatedAt(){
    return this->updated_at;
}

std::string vemt::db::SubmissionModel::toString(){
    std::stringstream ret;

    std::time_t __created_at = std::chrono::system_clock::to_time_t(this->getCreatedAt());
    const tm*    _created_at = std::localtime(&__created_at);

    std::time_t __updated_at = std::chrono::system_clock::to_time_t(this->getUpdatedAt());
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
