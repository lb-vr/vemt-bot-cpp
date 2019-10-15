#include "EntriesTable.hpp"
#include <sstream>

std::string vemt::db::EntriesTable::getTableName(){
    return std::string("entries");
}

vemt::db::EntriesTable::EntriesTable(const std::string & dbPath) : BaseTable(dbPath){
}

vemt::db::EntryModel vemt::db::EntriesTable::getById(const int id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::EntryModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "E.id AS id, "
            <<  "E.discord_user_id AS discord_user_id, "
            <<  "E.current_phase AS current_phase, "
            <<  "E.query_status_message_id AS query_status_message_id, "
            <<  "E.working_status_message_id AS working_status_message_id, "
            <<  "STRFTIME('%s', E.created_at) AS created_at, "
            <<  "STRFTIME('%s', E.updated_at) AS updated_at "
            <<  "FROM " << vemt::db::EntriesTable::getTableName() << " AS E "
            <<  "where E.id=? "
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
            auto _discord_user_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));


			auto _package_url = vemt::type::StringParam();
			_package_url.setAsCStr(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
            auto _current_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 3));
            auto _query_status_message_id = vemt::type::IntParam(sqlite3_column_int(stmt, 4));
            auto _working_status_message_id = vemt::type::IntParam(sqlite3_column_int(stmt, 5));
			auto __created_at = vemt::type::StringParam();
			__created_at.setAsCStr(sqlite3_column_text(stmt, 6), sqlite3_column_bytes(stmt, 6));
			auto _created_at = vemt::type::DatetimeParam();
			_created_at.setAsString(__created_at.get());
			auto __updated_at = vemt::type::StringParam();
			__updated_at.setAsCStr(sqlite3_column_text(stmt, 7), sqlite3_column_bytes(stmt, 7));
			auto _updated_at = vemt::type::DatetimeParam();
			_updated_at.setAsString(__updated_at.get());
            retValue.push_back(
                EntryModel(
                    _id,
                    _discord_user_id,
                    _package_url,
                    _current_phase,
                    _query_status_message_id,
                    _working_status_message_id,
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
    return retValue.at(0);
}
