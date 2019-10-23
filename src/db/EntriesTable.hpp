#ifndef VEMT_DB_ENTRIESTABLE_INCLUDED
#define VEMT_DB_ENTRIESTABLE_INCLUDED
#include "BaseTable.hpp"
#include "EntryModel.hpp"

namespace vemt
{
namespace db
{

class EntriesTable : public BaseTable
{
public:
    static std::string getTableName();
    EntriesTable(const std::string & dbPath);
    std::vector<vemt::db::EntryModel> getById(const int id);
    std::vector<vemt::db::EntryModel> getByDiscordUid(const int64_t id);
    std::vector<vemt::db::EntryModel> insert(const vemt::db::EntryModel & candidate);
};
} // namespace db
} // namespace vemt
#endif
