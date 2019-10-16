#ifndef VEMT_DB_SUBMISSIONSTABLE_INCLUDED
#define VEMT_DB_SUBMISSIONSTABLE_INCLUDED
#include <sstream>
#include "sqlite3.h"
#include "BaseTable.hpp"
#include "SubmissionModel.hpp"

namespace vemt
{
namespace db
{

class SubmissionsTable : public BaseTable
{
public:
    static std::string getTableName();
    SubmissionsTable(const std::string & dbPath) noexcept;
    std::vector<vemt::db::SubmissionModel> getById(const long int id);
    std::vector<vemt::db::SubmissionModel> getByDiscordUid(const int64_t id);
};
} // namespace db
} // namespace vemt
#endif
