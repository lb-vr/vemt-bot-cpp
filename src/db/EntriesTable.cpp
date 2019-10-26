#include "EntriesTable.hpp"
#include "Statement.hpp"
#include "Transaction.hpp"
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
			fetched_value.at("id").getAsInt(),
			fetched_value.at("discord_user_id").getAsInt(),
			fetched_value.at("current_phase").getAsPhase(),
			fetched_value.at("contact_channel_id").getAsInt(),
			fetched_value.at("questionary_message_id").getAsInt(),
			fetched_value.at("created_at").getAsDatetime(),
			fetched_value.at("updated_at").getAsDatetime()
		);
	}
    return retValue;
}

std::vector<vemt::db::EntryModel> vemt::db::EntriesTable::getByDiscordUid(const int64_t id)
{
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

	Statement stmt(pdb, sql_ss.str());
	stmt.bindInt(1, id);
	while (stmt.step()) {
		auto fetched = stmt.fetch();
		retValue.emplace_back(
			fetched.at("id").getAsInt(),
			fetched.at("discord_user_id").getAsInt(),
			fetched.at("current_phase").getAsPhase(),
			fetched.at("contact_channel_id").getAsInt(),
			fetched.at("questionary_message_id").getAsInt(),
			fetched.at("created_at").getAsDatetime(),
			fetched.at("updated_at").getAsDatetime()
		);
	}
    return retValue;
}

std::vector<vemt::db::EntryModel> vemt::db::EntriesTable::insert(const vemt::db::EntryModel & candidate) {

	std::vector<vemt::db::EntryModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "INSERT "
            <<  "INTO " << vemt::db::EntriesTable::getTableName() << " ("
            <<  "discord_user_id, "
            <<  "current_phase, "
            <<  "contact_channel_id, "
            <<  "questionary_message_id"
            <<  ") VALUES (?, ?, ?, ?) ";

	{
		Transaction transaction(pdb);
		Statement stmt(pdb, sql_ss.str());
		stmt.bindInt(1, candidate.getDiscordUid());
		stmt.bindPhase(2, candidate.getCurrentPhase());
		stmt.bindInt(3, candidate.getContactChannelId());
		stmt.bindInt(4, candidate.getQuestionaryMessageId());

		stmt.step();

		auto last_inserted_id = ::sqlite3_last_insert_rowid(pdb);
		auto last_inserted = this->getById(last_inserted_id);
		if (last_inserted.size() != 1) throw DatabaseException(-1, "Failed to get last inserted item.");
		retValue.push_back(last_inserted.at(0));

		transaction.commit();
	}
    return retValue;
}
