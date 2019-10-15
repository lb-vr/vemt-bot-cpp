#include "BaseTable.hpp"

vemt::db::BaseTable::BaseTable(const std::string & dbPath)
{
    setlocale(LC_ALL, "");
    this->tableName = "";
    this->databasePath = dbPath;
    this->pdb = NULL;

    auto err = ::sqlite3_open(this->databasePath.c_str(), &pdb);
    if (err != SQLITE_OK){
        std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
        throw std::exception();
    }
    err = ::sqlite3_exec(pdb, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    if (err != SQLITE_OK){
        std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
        throw std::exception();
    }
}

vemt::db::BaseTable::~BaseTable()
{
    ::sqlite3_close_v2(pdb);
    this->pdb = NULL;
}

::sqlite3_stmt * vemt::db::BaseTable::prepareStatement(const std::string & sql)
{
    ::sqlite3_stmt *stmt = NULL;
    auto err = ::sqlite3_prepare_v2(
        pdb,
        sql.c_str(),
        sql.length(),
        &stmt,
        NULL
    );
    if(err != SQLITE_OK){
        std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
        throw std::exception();
    }
    return stmt;
}
void vemt::db::BaseTable::finalizeStatement(::sqlite3_stmt * stmt){
    ::sqlite3_finalize(stmt);
}
