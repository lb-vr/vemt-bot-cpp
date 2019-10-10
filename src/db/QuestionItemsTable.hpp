#ifndef VEMT_DB_QUESTIONITEMSTABLE_INCLUDED
#define VEMT_DB_QUESTIONITEMSTABLE_INCLUDED
#include <sstream>
#include "sqlite3.h"
#include "BaseTable.hpp"
#include "QuestionItemModel.hpp"

namespace vemt
{
namespace db
{

class QuestionItemsTable : public BaseTable
{
public:
    static std::string getTableName();
    QuestionItemsTable(const std::string & dbPath) noexcept;
    vemt::db::SubmissionModel getById(const long int id);
    std::vector<vemt::db::SubmissionModel> getAll();
    int put(vemt::db::SubmissionModel candidate);
    //vemt::db::SubmissionModel __update(vemt::db::SubmissionModel);
    //int __deleteById(unsigned long int id);

protected:
    std::string tableName;
    std::string databasePath;
    sqlite3 *pdb;
};
} // namespace db
} // namespace vemt
#endif
