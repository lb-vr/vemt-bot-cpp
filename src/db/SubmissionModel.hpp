#ifndef VEMT_DB_SUBMISSIONMODEL_INCLUDED
#define VEMT_DB_SUBMISSIONMODEL_INCLUDED
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "type/Type.hpp"
namespace vemt
{
namespace db
{

class SubmissionModel
{
public:
    SubmissionModel(
        const vemt::type::IntParam id,
        const vemt::type::IntParam entry_id,
        const vemt::type::StringParam package_url,
        const vemt::type::IntParam current_phase,
        const vemt::type::DatetimeParam created_at,
        const vemt::type::DatetimeParam updated_at
    ) noexcept;
    SubmissionModel(
        const vemt::type::IntParam entry_id,
        const vemt::type::StringParam package_url,
        const vemt::type::IntParam current_phase
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
    vemt::type::IntParam id;
    vemt::type::IntParam entry_id;
    vemt::type::StringParam package_url;
    vemt::type::IntParam current_phase;
    vemt::type::DatetimeParam created_at;
    vemt::type::DatetimeParam updated_at;
};
} // namespace db
} // namespace vemt
#endif
