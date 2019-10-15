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
    vemt::db::QuestionItemModel getById(const int id);
    std::vector<vemt::db::QuestionItemModel> getAll();
    int put(vemt::db::QuestionItemModel candidate);
    //vemt::db::QuestionItemModel __update(vemt::db::QuestionItemModel);
    //int __deleteById(unsigned long int id);

private:
    static std::string getChoicesTableName();
    std::vector<vemt::type::WstringParam> getChoices(const int id);

};
} // namespace db
} // namespace vemt
#endif
