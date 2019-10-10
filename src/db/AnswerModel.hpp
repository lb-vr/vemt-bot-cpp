#ifndef VEMT_DB_ANSWERMODEL_INCLUDED
#define VEMT_DB_ANSWERMODEL_INCLUDED
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

class AnswerModel
{
public:
    static const std::string tableName;
    AnswerModel(
        long int id,
        long int discord_uid,
        std::string package_url,
        vemt::Phase current_phase,
        std::chrono::system_clock::time_point created_at,
        std::chrono::system_clock::time_point updated_at
    );
    ~AnswerModel(){}

    long int getId();
    long int getEntryId();
    long int getQuestionItemId();
    std::string getItemValue();
    std::chrono::system_clock::time_point getCreatedAt();
    std::chrono::system_clock::time_point getUpdatedAt();
    std::string toString();

private:
    long int id;
    long int entry_id;
    long int question_item_id;
    std::string item_value;
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
};
} // namespace db
} // namespace vemt
#endif
