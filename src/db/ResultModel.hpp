#ifndef VEMT_DB_RESULTMODEL_INCLUDED
#define VEMT_DB_RESULTMODEL_INCLUDED
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

class ResultModel
{
public:
    static const std::string tableName;
    ResultModel(
        long int id,
        long int discord_uid,
        std::string package_url,
        vemt::Phase current_phase,
        std::chrono::system_clock::time_point created_at,
        std::chrono::system_clock::time_point updated_at
    );
    ~ResultModel(){}

    long int getId();
    long int getSubmissionId();
    vemt::Phase getAsPhase();
    bool getIsPassed();
    std::string getLogText();
    std::chrono::system_clock::time_point getCreatedAt();
    std::string toString();

private:
    long int id;
    long int submission_id;
    vemt::Phase as_phase;
    bool is_passed;
    std::string log_text;
    std::chrono::system_clock::time_point created_at;
};
} // namespace db
} // namespace vemt
#endif
