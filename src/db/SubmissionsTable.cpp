#include "SubmissionsTable.hpp"

std::string vemt::db::SubmissionsTable::getTableName(){
    return std::string("submissions");
}

vemt::db::SubmissionsTable::SubmissionsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

vemt::db::SubmissionModel vemt::db::SubmissionsTable::getById(const long int id)
{
    long int _id;
    long int _discord_user_id;
    std::string _package_url;
    int _current_phase;
    int _created_at;
    int _updated_at;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "S.id AS id, "
            <<  "S.entry_id AS entry_id, "
            <<  "S.package_url AS package_url, "
            <<  "S.current_phase AS current_phase, "
            <<  "STRFTIME('%s', S.created_at) AS created_at, "
            <<  "STRFTIME('%s', S.updated_at) AS updated_at "
            <<  "FROM " << vemt::db::SubmissionsTable::getTableName() << " AS S "
            <<  "WHERE id=? "
            <<  "LIMIT 1";
    try{
        auto err = this->prepareStatement(sql_ss.str());
        if (err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
            throw std::exception();
        }
        err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_ROW) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        _id              = sqlite3_column_int(stmt, 0);
        _discord_user_id = sqlite3_column_int(stmt, 1);
        _package_url     = "";//this->char2str(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
        _created_at      = sqlite3_column_int(stmt, 3);
        _updated_at      = sqlite3_column_int(stmt, 4);

        return SubmissionModel(
            _id,
            _discord_user_id,
            _package_url,
            1,
            std::chrono::system_clock::from_time_t(_created_at),
            std::chrono::system_clock::from_time_t(_updated_at)
        );
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
}

std::vector<vemt::db::SubmissionModel> vemt::db::SubmissionsTable::getByDiscordUid(const long int discord_user_id)
{
    ::sqlite3_stmt *stmt = NULL;
    int id = 10;
    std::vector<vemt::db::SubmissionModel> retValue;
    /*
    try{
        
        auto err = this->prepareStatement(
            "SELECT "
                "E.entry_id AS entry_id, "
                "E.discord_user_id AS discord_user_id, "
                "E.package_url AS package_url, "
                "STRFTIME('%s', E.created_at) AS created_at, "
                "STRFTIME('%s', E.updated_at) AS updated_at "
            "FROM entries AS E "
            "WHERE entry_id=? "
            "LIMIT 1");
        if (err != SQLITE_OK){
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
                vemt::Phase::kEntry,
                std::chrono::system_clock::from_time_t(created_at),
                std::chrono::system_clock::from_time_t(updated_at));
            retValue.push_back(entry);
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement();
    */
    return retValue;
}
