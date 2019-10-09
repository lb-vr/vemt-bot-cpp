#ifndef VEMT_DB_BASETABLE_INCLUDED
#define VEMT_DB_BASETABLE_INCLUDED
#define SQLITE_CORE
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <exception>
namespace vemt
{
namespace db
{

class BaseTable
{
public:
    BaseTable(const std::string & dbPath) noexcept;
    virtual ~BaseTable();

protected:
    std::string tableName;
    std::string databasePath;
    ::sqlite3 *pdb;

    int connectDatabase();
    std::string char2str(const unsigned char *c_str, size_t len);

};
} // namespace db
} // namespace vemt
#endif
