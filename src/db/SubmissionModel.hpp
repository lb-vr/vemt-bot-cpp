#ifndef VEMT_DB_SUBMISSIONMODEL_INCLUDED
#define VEMT_DB_SUBMISSIONMODEL_INCLUDED
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../Phase.hpp"
namespace vemt
{
namespace db
{

class SubmissionModel
{
public:
    SubmissionModel(
        const long int id,
        const long int discord_uid,
        const std::string package_url,
        const vemt::Phase current_phase,
        const std::chrono::system_clock::time_point created_at,
        const std::chrono::system_clock::time_point updated_at
    ) noexcept;
    ~SubmissionModel(){}

    long int getId();
    long int getDiscordUid();
    std::string getPackageUrl();
    vemt::Phase  getCurrentPhase();
    std::chrono::system_clock::time_point getCreatedAt();
    std::chrono::system_clock::time_point getUpdatedAt();
    std::string toString();

private:
    long int id;
    long int discord_uid;
    std::string package_url;
    vemt::Phase current_phase;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
};
} // namespace db
} // namespace vemt
#endif
