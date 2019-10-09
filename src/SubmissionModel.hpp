#ifndef VEMT_DB_SUBMISSIONMODEL_INCLUDED
#define VEMT_DB_SUBMISSIONMODEL_INCLUDED
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
namespace vemt
{
namespace db
{

class SubmissionModel
{
public:
    SubmissionModel(
        unsigned long int id,
        unsigned long int discord_uid,
        std::string package_url,
        std::chrono::system_clock::time_point created_at,
        std::chrono::system_clock::time_point updated_at
    );
    ~SubmissionModel(){}

    unsigned long int getId();
    unsigned long int getDiscordUid();
    std::string getPackageUrl();
    std::chrono::system_clock::time_point getCreatedAt();
    std::chrono::system_clock::time_point getUpdatedAt();

    std::string toString();

    static const std::string tableName;

private:
    unsigned long int id;
    unsigned long int discord_uid;
    std::string package_url;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
};
} // namespace db
} // namespace vemt
#endif
