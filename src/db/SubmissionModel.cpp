#include "SubmissionModel.hpp"

vemt::db::SubmissionModel::SubmissionModel(
        unsigned long int id,
        unsigned long int discord_uid,
        std::string package_url,
        std::chrono::system_clock::time_point created_at,
        std::chrono::system_clock::time_point updated_at
){
    this->id = id;
    this->discord_uid = discord_uid;
    this->package_url = package_url;
    this->created_at = created_at;
    this->updated_at = updated_at;
}

unsigned long int vemt::db::SubmissionModel::getId(){
    return this->id;
}
unsigned long int vemt::db::SubmissionModel::getDiscordUid(){
    return this->discord_uid;
}
std::string vemt::db::SubmissionModel::getPackageUrl(){
    return this->package_url;
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

    ret << "{model: 'Entry', "
        << "id: "<< this->getId() << ", "
        << "discord_uid: " << this->getDiscordUid() << ", "
        << "package_url: '" << this->getPackageUrl() << "', "
        << "created_at: '" << std::put_time(_created_at, "%F_%T") << "', "
        << "updated_at: '" << std::put_time(_updated_at, "%F_%T") << "'}";
    return ret.str();
}
