#ifndef VEMT_DB_SUBMISSIONMODEL_INCLUDED
#define VEMT_DB_SUBMISSIONMODEL_INCLUDED
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "type/Param.hpp"
namespace vemt
{
namespace db
{

class SubmissionModel
{
public:
    SubmissionModel(
        const vemt::db::type::IntParam id,
        const vemt::db::type::IntParam discord_uid,
        const vemt::db::type::StringParam package_url,
        const vemt::db::type::IntParam current_phase,
        const vemt::db::type::DatetimeParam created_at,
        const vemt::db::type::DatetimeParam updated_at
    ) noexcept;
    SubmissionModel(
        const vemt::db::type::IntParam discord_uid,
        const vemt::db::type::StringParam package_url,
        const vemt::db::type::IntParam current_phase
    ) noexcept;
    ~SubmissionModel(){}

    int getId();
    int getDiscordUid();
    std::string getPackageUrl();
    int  getCurrentPhase();
    time_t getCreatedAt();
    time_t getUpdatedAt();

    void setId(int v);
    void setDiscordUid(int v);
    void setPackageUrl(std::string v);
    void setCurrentPhase(int v);

    std::string toString();

private:
    vemt::db::type::IntParam id;
    vemt::db::type::IntParam discord_uid;
    vemt::db::type::StringParam package_url;
    vemt::db::type::IntParam current_phase;
    vemt::db::type::DatetimeParam created_at;
    vemt::db::type::DatetimeParam updated_at;
};
} // namespace db
} // namespace vemt
#endif
