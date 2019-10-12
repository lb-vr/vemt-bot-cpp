#include "BaseTable.hpp"

vemt::db::BaseTable::BaseTable(const std::string & dbPath) noexcept
{
    setlocale(LC_ALL, "");
    this->tableName = "";
    this->databasePath = dbPath;
    this->pdb = NULL;
    this->stmt = NULL;
}

vemt::db::BaseTable::~BaseTable()
{
    this->finalizeStatement();
    ::sqlite3_close_v2(pdb);
    this->pdb = NULL;
}

int vemt::db::BaseTable::prepareStatement(const std::string & sql)
{
    auto err = ::sqlite3_open(this->databasePath.c_str(), &pdb);
    if (err != SQLITE_OK){
        std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
        throw std::exception();
    }
    err = ::sqlite3_prepare_v2(
        pdb,
        sql.c_str(),
        -1,
        &stmt,
        NULL
    );
    if(err != SQLITE_OK){
        std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
        throw std::exception();
    }
    return err;
}
int vemt::db::BaseTable::finalizeStatement(){
    if(this->stmt != NULL){
        ::sqlite3_finalize(stmt);
        this->stmt = NULL;
    }
}
