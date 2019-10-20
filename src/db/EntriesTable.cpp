#include "EntriesTable.hpp"
#include "Statement.hpp"
#include <sstream>

std::string vemt::db::EntriesTable::getTableName(){
    return std::string("entries");
}

vemt::db::EntriesTable::EntriesTable(const std::string & dbPath) : BaseTable(dbPath){
}

std::vector<vemt::db::EntryModel> vemt::db::EntriesTable::getById(const int id)
{
    std::vector<vemt::db::EntryModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "E.id, "
            <<  "E.discord_user_id, "
            <<  "E.current_phase, "
            <<  "E.contact_channel_id, "
            <<  "E.questionary_message_id, "
            <<  "E.created_at, "
            <<  "E.updated_at "
            <<  "FROM " << vemt::db::EntriesTable::getTableName() << " AS E "
            <<  "where E.id=? "
            <<  "LIMIT 1";

	Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, id);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.emplace_back(
			fetched_value.at("entries.id").getAsInt(),
			fetched_value.at("entries.discord_user_id").getAsInt(),
			fetched_value.at("entries.current_phase").getAsPhase(),
			fetched_value.at("entries.contact_channel_id").getAsInt(),
			fetched_value.at("entries.questionary_message_id").getAsInt(),
			fetched_value.at("entries.created_at").getAsDatetime(),
			fetched_value.at("entries.updated_at").getAsDatetime()
		);
	}
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
            <<  "E.contact_channel_id AS contact_channel_id, "
            <<  "E.questionary_message_id AS questionary_message_id, "
            <<  "E.created_at AS created_at, "
            <<  "E.updated_at AS updated_at "
            <<  "FROM " << vemt::db::EntriesTable::getTableName() << " AS E "
            <<  "where E.discord_user_id=? "
            <<  "LIMIT 1";
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int64(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        while ((err = ::sqlite3_step(stmt)) == SQLITE_ROW) {
            auto _id = vemt::type::IntParam(sqlite3_column_int64(stmt, 0));
            auto _discord_user_id = vemt::type::IntParam(sqlite3_column_int64(stmt, 1));
			auto _current_phase = vemt::type::PhaseParam();
            _current_phase.setAsInt(sqlite3_column_int(stmt, 2));
            auto _contact_channel_id = vemt::type::IntParam(sqlite3_column_int64(stmt, 3));
            auto _questionary_message_id = vemt::type::IntParam(sqlite3_column_int64(stmt, 4));
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
                    _contact_channel_id,
                    _questionary_message_id,
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
            <<  "contact_channel_id, "
            <<  "questionary_message_id"
            <<  ") VALUES (?, ?, ?, ?) "
            ;
    try{
        auto err = sqlite3_exec(pdb, "BEGIN;", nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "CANNOT BEGIN TRANSACTION" << std::endl;
            throw std::exception();
        }
        stmt = this->prepareStatement(sql_ss.str());
        err = ::sqlite3_bind_int64(stmt, 1, candidate.getDiscordUid().get());
        err |= ::sqlite3_bind_int(stmt, 2, candidate.getCurrentPhase().getAsInt());
        err |= ::sqlite3_bind_int64(stmt, 3, candidate.getContactChannelId().get());
        err |= ::sqlite3_bind_int64(stmt, 4, candidate.getQuestionaryMessageId().get());
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
