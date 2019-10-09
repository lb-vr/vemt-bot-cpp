#include "SubmissionsTable.hpp"

std::string vemt::db::SubmissionsTable::getTableName(){
    return std::string("submissions");
}

vemt::db::SubmissionsTable::SubmissionsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::SubmissionModel> vemt::db::SubmissionsTable::getById(const unsigned long int id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::SubmissionModel> retValue;
    try{
        
        auto err = ::sqlite3_open("develop.sqlite3", &pdb);
        if (err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
            throw std::exception();
        }
        err = ::sqlite3_prepare_v2(
            pdb,
            "SELECT "
                "E.entry_id AS entry_id, "
                "E.discord_user_id AS discord_user_id, "
                "E.package_url AS package_url, "
                "STRFTIME('%s', E.created_at) AS created_at, "
                "STRFTIME('%s', E.updated_at) AS updated_at "
            "FROM entries AS E "
            "WHERE entry_id=? "
            "LIMIT 1",
            -1,
            &stmt,
            NULL
        );
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
            throw std::exception();
        }
        ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        while (::sqlite3_step(stmt) == SQLITE_ROW) {
            auto entry_id = sqlite3_column_int(stmt, 0);
            auto discord_user_id = sqlite3_column_int(stmt, 1);
            auto package_url = this->char2str(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
            auto created_at = sqlite3_column_int(stmt, 3);
            auto updated_at = sqlite3_column_int(stmt, 4);
            auto entry = SubmissionModel(
                entry_id,
                discord_user_id,
                package_url,
                std::chrono::system_clock::from_time_t(created_at),
                std::chrono::system_clock::from_time_t(updated_at));
            std::cerr << entry.toString() << std::endl;
            retValue.push_back(entry);
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    ::sqlite3_finalize(stmt);
    ::sqlite3_close(pdb);

    return retValue;
}
