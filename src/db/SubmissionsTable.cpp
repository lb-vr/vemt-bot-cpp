#include "SubmissionsTable.hpp"

std::string vemt::db::SubmissionsTable::getTableName(){
    return std::string("submissions");
}

vemt::db::SubmissionsTable::SubmissionsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

vemt::db::SubmissionModel vemt::db::SubmissionsTable::getById(const long int id)
{
    ::sqlite3_stmt *stmt = NULL;
    vemt::db::type::IntParam _id;
    vemt::db::type::IntParam _discord_user_id;
    vemt::db::type::StringParam _package_url;
    vemt::db::type::IntParam _current_phase;
    vemt::db::type::DatetimeParam _created_at;
    vemt::db::type::DatetimeParam _updated_at;
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
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_ROW) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        _id              = vemt::db::type::IntParam(sqlite3_column_int(stmt, 0));
        _discord_user_id = vemt::db::type::IntParam(sqlite3_column_int(stmt, 1));
		_package_url = vemt::db::type::StringParam();
		_package_url.setAsCStr(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
        _current_phase = vemt::db::type::IntParam(sqlite3_column_int(stmt, 3));
		auto __created_at = vemt::db::type::StringParam(); 
		__created_at.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
		_created_at = vemt::db::type::DatetimeParam();
		_created_at.setAsString(__created_at.get());
		auto __updated_at = vemt::db::type::StringParam();
		__updated_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
		_updated_at = vemt::db::type::DatetimeParam();
		_updated_at.setAsString(__updated_at.get());
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return SubmissionModel(
        _id,
        _discord_user_id,
        _package_url,
        _current_phase,
        _created_at,
        _updated_at
    );
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
