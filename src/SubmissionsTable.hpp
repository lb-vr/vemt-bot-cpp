#ifndef VEMT_DB_ENTRYTABLE_INCLUDED
#define VEMT_DB_ENTRYTABLE_INCLUDED
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
    std::vector<vemt::db::SubmissionModel> getById(const unsigned long int id);
    std::vector<vemt::db::SubmissionModel> getByDiscordUid();
    std::vector<vemt::db::SubmissionModel> __update(std::vector<vemt::db::SubmissionModel>);
    unsigned int __insert(std::vector<vemt::db::SubmissionModel>);
    unsigned int __delete(std::vector<vemt::db::SubmissionModel>);

protected:
    std::string tableName;
    std::string databasePath;
    sqlite3 *pdb;
};
} // namespace db
} // namespace vemt
#endif
