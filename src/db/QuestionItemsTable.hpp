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
    std::vector<vemt::db::QuestionItemModel> getById(const int id);
    std::vector<vemt::db::QuestionItemModel> getAll();
    std::vector<vemt::db::QuestionItemModel> replaceAll(std::vector<vemt::db::QuestionItemModel> candidates);

private:
    static std::string getChoicesTableName();
    std::vector<vemt::type::WstringParam> getChoices(const int id);

};
} // namespace db
} // namespace vemt
#endif
