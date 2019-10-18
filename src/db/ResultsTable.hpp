#ifndef VEMT_DB_RESULTSSTABLE_INCLUDED
#define VEMT_DB_RESULTSSTABLE_INCLUDED
#include <sstream>
#include "sqlite3.h"
#include "BaseTable.hpp"
#include "ResultModel.hpp"

namespace vemt
{
namespace db
{

class ResultsTable : public BaseTable
{
public:
    static std::string getTableName();
    ResultsTable(const std::string & dbPath) noexcept;
    std::vector<vemt::db::ResultModel> getById(const int id);
    std::vector<vemt::db::ResultModel> getBySubmissionId(const int id);
    std::vector<vemt::db::ResultModel> getByDiscordUserId(const int id);
    std::vector<vemt::db::ResultModel> getAll();

private:
    static std::string getChoicesTableName();
    std::vector<vemt::type::WstringParam> getChoices(const int id);

};
} // namespace db
} // namespace vemt
#endif
