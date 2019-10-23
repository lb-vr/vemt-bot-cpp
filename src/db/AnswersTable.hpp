#ifndef VEMT_DB_ANSWERSTABLE_INCLUDED
#define VEMT_DB_ANSWERSTABLE_INCLUDED
#include <sstream>
#include "sqlite3.h"
#include "BaseTable.hpp"
#include "AnswerModel.hpp"

namespace vemt
{
namespace db
{

class AnswersTable : public BaseTable
{
public:
    static std::string getTableName();
    AnswersTable(const std::string & dbPath) noexcept;
    std::vector<vemt::db::AnswerModel> getByDiscordUserId(const int discord_user_id);
};
} // namespace db
} // namespace vemt
#endif
