#include "BaseTable.hpp"

vemt::db::BaseTable::BaseTable(const std::string & dbPath) noexcept
{
    setlocale(LC_ALL, "");
    this->tableName = "";
    this->databasePath = dbPath;
    this->pdb = NULL;
}

vemt::db::BaseTable::~BaseTable()
{
    ::sqlite3_close_v2(pdb);
    this->pdb = NULL;
}

int vemt::db::BaseTable::connectDatabase()
{
    return ::sqlite3_open(this->databasePath.c_str(), &pdb);
}

std::string vemt::db::BaseTable::char2str(const unsigned char *c_str, size_t len){
    std::string ret = "";
    for (size_t l = 0; l < len; l++){
        ret.push_back(c_str[l]);
    }
    return ret;
}

