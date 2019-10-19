#include "ResultsTable.hpp"
#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"

std::string vemt::db::ResultsTable::getTableName(){
    return std::string("results");
}

vemt::db::ResultsTable::ResultsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::ResultModel> vemt::db::ResultsTable::getById(const int id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::ResultModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "R.id AS id, "
            <<  "R.submission_id AS submission_id, "
            <<  "R.as_phase AS as_phase, "
            <<  "R.is_passed AS is_passed, "
            <<  "R.log_text AS log_text, "
            <<  "R.created_at AS created_at, "
            <<  "FROM " << vemt::db::ResultsTable::getTableName() << " AS R "
            <<  "WHERE R.id=? "
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
        auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
        auto _submission_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));
        auto _as_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 2));
        auto _is_passed = vemt::type::BoolParam(sqlite3_column_int(stmt, 3));
		auto _log_text = vemt::type::StringParam();
		_log_text.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
		auto __created_at = vemt::type::StringParam();
		__created_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
		auto _created_at = vemt::type::DatetimeParam();
		_created_at.setAsString(__created_at.get());

        retValue.push_back(ResultModel(
            _id,
            _submission_id,
            _as_phase,
            _is_passed,
            _log_text,
            _created_at
        ));
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}

std::vector<vemt::db::ResultModel> vemt::db::ResultsTable::getBySubmissionId(const int submission_id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::ResultModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "R.id AS id, "
            <<  "R.submission_id AS submission_id, "
            <<  "R.as_phase AS as_phase, "
            <<  "R.is_passed AS is_passed, "
            <<  "R.log_text AS log_text, "
            <<  "R.created_at AS created_at, "
            <<  "FROM " << vemt::db::ResultsTable::getTableName() << " AS R "
            <<  "WHERE R.submission_id=? "
            ;
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, submission_id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_ROW) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
        auto _submission_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));
        auto _as_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 2));
        auto _is_passed = vemt::type::BoolParam(sqlite3_column_int(stmt, 3));
		auto _log_text = vemt::type::StringParam();
		_log_text.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
		auto __created_at = vemt::type::StringParam();
		__created_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
		auto _created_at = vemt::type::DatetimeParam();
		_created_at.setAsString(__created_at.get());

        retValue.push_back(ResultModel(
            _id,
            _submission_id,
            _as_phase,
            _is_passed,
            _log_text,
            _created_at
        ));
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}

std::vector<vemt::db::ResultModel> vemt::db::ResultsTable::getByDiscordUserId(const int discord_user_id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::ResultModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "R.id AS id, "
            <<  "R.submission_id AS submission_id, "
            <<  "R.as_phase AS as_phase, "
            <<  "R.is_passed AS is_passed, "
            <<  "R.log_text AS log_text, "
            <<  "R.created_at AS created_at, "
            <<  "FROM " << vemt::db::ResultsTable::getTableName() << " AS R "
            <<  "INNER JOIN " << vemt::db::SubmissionsTable::getTableName() << " AS S "
            <<  "ON R.submission_id = S.id "
            <<  "INNER JOIN " << vemt::db::EntriesTable::getTableName() << " AS E "
            <<  "ON S.entry_id = E.id"
            <<  "WHERE E.discord_user_id=? "
            ;
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, discord_user_id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_ROW) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
        auto _submission_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));
        auto _as_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 2));
        auto _is_passed = vemt::type::BoolParam(sqlite3_column_int(stmt, 3));
		auto _log_text = vemt::type::StringParam();
		_log_text.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
		auto __created_at = vemt::type::StringParam();
		__created_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
		auto _created_at = vemt::type::DatetimeParam();
		_created_at.setAsString(__created_at.get());

        retValue.push_back(ResultModel(
            _id,
            _submission_id,
            _as_phase,
            _is_passed,
            _log_text,
            _created_at
        ));
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}
