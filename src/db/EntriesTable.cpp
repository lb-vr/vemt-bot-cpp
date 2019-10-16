#include "EntriesTable.hpp"
#include <sstream>

std::string vemt::db::EntriesTable::getTableName(){
    return std::string("entries");
}

vemt::db::EntriesTable::EntriesTable(const std::string & dbPath) : BaseTable(dbPath){
}

std::vector<vemt::db::EntryModel> vemt::db::EntriesTable::getById(const int id)
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
            auto _current_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 2));
            auto _query_status_message_id = vemt::type::IntParam(sqlite3_column_int(stmt, 3));
            auto _working_status_message_id = vemt::type::IntParam(sqlite3_column_int(stmt, 4));
			auto __created_at = vemt::type::StringParam();
			__created_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
			auto _created_at = vemt::type::DatetimeParam();
			_created_at.setAsString(__created_at.get());
			auto __updated_at = vemt::type::StringParam();
			__updated_at.setAsCStr(sqlite3_column_text(stmt, 6), sqlite3_column_bytes(stmt, 6));
			auto _updated_at = vemt::type::DatetimeParam();
			_updated_at.setAsString(__updated_at.get());
            retValue.push_back(
                EntryModel(
                    _id,
                    _discord_user_id,
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
    return retValue;
}

std::vector<vemt::db::EntryModel> vemt::db::EntriesTable::getByDiscordUid(const int64_t id)
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
            <<  "where E.discord_user_id=? "
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
            auto _current_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 2));
            auto _query_status_message_id = vemt::type::IntParam(sqlite3_column_int(stmt, 3));
            auto _working_status_message_id = vemt::type::IntParam(sqlite3_column_int(stmt, 4));
			auto __created_at = vemt::type::StringParam();
			__created_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));
			auto _created_at = vemt::type::DatetimeParam();
			_created_at.setAsString(__created_at.get());
			auto __updated_at = vemt::type::StringParam();
			__updated_at.setAsCStr(sqlite3_column_text(stmt, 6), sqlite3_column_bytes(stmt, 6));
			auto _updated_at = vemt::type::DatetimeParam();
			_updated_at.setAsString(__updated_at.get());
            retValue.push_back(
                EntryModel(
                    _id,
                    _discord_user_id,
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
    return retValue;
}

std::vector<vemt::db::EntryModel> vemt::db::EntriesTable::insert(const vemt::db::EntryModel & candidate)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::EntryModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "INSERT "
            <<  "INTO " << vemt::db::EntriesTable::getTableName() << " ("
            <<  "discord_user_id, "
            <<  "current_phase, "
            <<  "query_status_message_id, "
            <<  "working_status_message_id"
            <<  ") VALUES (?, ?, ?, ?) "
            ;
    try{
        auto err = sqlite3_exec(pdb, "BEGIN;", nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "CANNOT BEGIN TRANSACTION" << std::endl;
            throw std::exception();
        }
        stmt = this->prepareStatement(sql_ss.str());
        err = ::sqlite3_bind_int(stmt, 1, candidate.getDiscordUid());
        err |= ::sqlite3_bind_int(stmt, 2, candidate.getCurrentPhase());
        err |= ::sqlite3_bind_int(stmt, 3, candidate.getQueryStatusMessageId());
        err |= ::sqlite3_bind_int(stmt, 4, candidate.getWorkingStatusMessageId());
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_DONE) {
            std::cerr << __FILE__ << " : " << __LINE__ << "\t" << err << std::endl;
            throw std::exception();
        }
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
        retValue.push_back(
            *(this->getById(last_inserted_id).begin())
        );
        err = sqlite3_exec(pdb, "COMMIT;", nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << " CANNOT COMMIT TRANSACTION" << std::endl;
            throw std::exception();
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
        auto err = sqlite3_exec(pdb, "ROLLBACK;", nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << " CANNOT ROLLBACK TRANSACTION" << std::endl;
            throw std::exception();
        }
    }
    this->finalizeStatement(stmt);
    return retValue;
}
