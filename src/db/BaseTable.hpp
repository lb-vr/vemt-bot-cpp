#ifndef VEMT_DB_BASETABLE_INCLUDED
#define VEMT_DB_BASETABLE_INCLUDED
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
    BaseTable(const std::string & dbPath);
    virtual ~BaseTable();

protected:
    std::string tableName;
    std::string databasePath;
    ::sqlite3 *pdb;

    ::sqlite3_stmt * prepareStatement(const std::string & sql);
    void finalizeStatement(::sqlite3_stmt * stmt);
};
} // namespace db
} // namespace vemt
#endif
