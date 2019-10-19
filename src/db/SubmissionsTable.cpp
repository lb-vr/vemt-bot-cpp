#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"

std::string vemt::db::SubmissionsTable::getTableName(){
    return std::string("submissions");
}

vemt::db::SubmissionsTable::SubmissionsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::SubmissionModel> vemt::db::SubmissionsTable::getById(const long int id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::SubmissionModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "S.id AS id, "
            <<  "S.entry_id AS entry_id, "
            <<  "S.package_url AS package_url, "
            <<  "S.current_phase AS current_phase, "
            <<  "S.created_at AS created_at, "
            <<  "S.updated_at AS updated_at "
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
        while ((err = ::sqlite3_step(stmt)) == SQLITE_ROW) {
            auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
            auto _entry_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));
            auto _package_url = vemt::type::StringParam();
            auto _current_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 3));
            auto __created_at = vemt::type::StringParam(); 
            auto _created_at = vemt::type::DatetimeParam();
            auto __updated_at = vemt::type::StringParam();
            auto _updated_at = vemt::type::DatetimeParam();
            _package_url.setAsCStr(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
            __created_at.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
            _created_at.setAsString(__created_at.toString());
            __updated_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
            _updated_at.setAsString(__updated_at.toString());
            std::cerr << _created_at.toString() << "\t" << _updated_at.toString() << std::endl;
            retValue.push_back(
                SubmissionModel(
                    _id,
                    _entry_id,
                    _package_url,
                    _current_phase,
                    _created_at,
                    _updated_at
                )
            );
        }
        if (err != SQLITE_DONE) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}

std::vector<vemt::db::SubmissionModel> vemt::db::SubmissionsTable::getByDiscordUid(const int64_t id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::SubmissionModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "S.id AS id, "
            <<  "S.entry_id AS entry_id, "
            <<  "S.package_url AS package_url, "
            <<  "S.current_phase AS current_phase, "
            <<  "S.created_at AS created_at, "
            <<  "S.updated_at AS updated_at "
            <<  "FROM " << vemt::db::SubmissionsTable::getTableName() << " AS S "
            <<  "INNER JOIN " << vemt::db::EntriesTable::getTableName() << " AS E "
            <<  "ON S.entry_id=E.id "
            <<  "WHERE E.discord_user_id=? "
            ;
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        while ((err = ::sqlite3_step(stmt)) == SQLITE_ROW) {
            auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
            auto _entry_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));
            auto _package_url = vemt::type::StringParam();
            auto _current_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 3));
            auto __created_at = vemt::type::StringParam(); 
            auto _created_at = vemt::type::DatetimeParam();
            auto __updated_at = vemt::type::StringParam();
            auto _updated_at = vemt::type::DatetimeParam();
            _package_url.setAsCStr(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
            __created_at.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
            _created_at.setAsString(__created_at.toString());
            __updated_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
            _updated_at.setAsString(__updated_at.toString());
            std::cerr << _created_at.toString() << "\t" << _updated_at.toString() << std::endl;
            retValue.push_back(
                SubmissionModel(
                    _id,
                    _entry_id,
                    _package_url,
                    _current_phase,
                    _created_at,
                    _updated_at
                )
            );
        }
        if (err != SQLITE_DONE) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}
