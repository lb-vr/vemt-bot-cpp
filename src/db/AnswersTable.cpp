#include "AnswersTable.hpp"
#include "EntriesTable.hpp"
#include "Statement.hpp"
#include "Transaction.hpp"
#include <sstream>

std::string vemt::db::AnswersTable::getTableName(){
    return std::string("answers");
}

vemt::db::AnswersTable::AnswersTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::AnswerModel> vemt::db::AnswersTable::getById(const int id)
{
    std::vector<vemt::db::AnswerModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "A.id AS id, "
            <<  "A.entry_id AS entry_id, "
            <<  "A.question_item_id AS question_item_id, "
            <<  "A.item_value AS item_value, "
            <<  "STRFTIME('%Y-%m-%d %H:%M:%S', A.created_at) AS created_at, "
            <<  "A.updated_at AS updated_at "
            <<  "FROM " << vemt::db::AnswersTable::getTableName() << " AS A "
            <<  "WHERE E.id=?"
            ;
    Statement stmt(this->pdb, sql_ss.str());
    stmt.bindInt(1, id);
    while (stmt.step()) {
        auto fetched_value = stmt.fetch();
        retValue.emplace_back(
            fetched_value.at("id").getAsInt(),
            fetched_value.at("entry_id").getAsInt(),
            fetched_value.at("question_item_id").getAsInt(),
            fetched_value.at("item_value").getAsString(),
            fetched_value.at("created_at").getAsDatetime(),
            fetched_value.at("updated_at").getAsDatetime()
        );
    }
    return retValue;
}

std::vector<vemt::db::AnswerModel> vemt::db::AnswersTable::getByDiscordUserId(const int discord_user_id)
{
    std::vector<vemt::db::AnswerModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "A.id AS id, "
            <<  "A.entry_id AS entry_id, "
            <<  "A.question_item_id AS question_item_id, "
            <<  "A.item_value AS item_value, "
            <<  "STRFTIME('%Y-%m-%d %H:%M:%S', A.created_at) AS created_at, "
            <<  "A.updated_at AS updated_at "
            <<  "FROM " << vemt::db::AnswersTable::getTableName() << " AS A "
            <<  "INNER JOIN " << vemt::db::EntriesTable::getTableName() << " AS E "
            <<  "ON A.entry_id = E.id "
            <<  "WHERE E.discord_user_id=?"
            ;
    Statement stmt(this->pdb, sql_ss.str());
    stmt.bindInt(1, discord_user_id);
    while (stmt.step()) {
        auto fetched_value = stmt.fetch();
        retValue.emplace_back(
            fetched_value.at("id").getAsInt(),
            fetched_value.at("entry_id").getAsInt(),
            fetched_value.at("question_item_id").getAsInt(),
            fetched_value.at("item_value").getAsString(),
            fetched_value.at("created_at").getAsDatetime(),
            fetched_value.at("updated_at").getAsDatetime()
        );
    }
    return retValue;
}

std::vector<vemt::db::AnswerModel> vemt::db::AnswersTable::insert(const vemt::db::AnswerModel & candidate)
{
	std::vector<vemt::db::AnswerModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "INSERT "
            <<  "INTO " << vemt::db::AnswersTable::getTableName() << " ("
            <<  "entry_id, "
            <<  "question_item_id, "
            <<  "item_value "
            <<  ") VALUES (?, ?, ?) ";

	{
		Transaction transaction(pdb);
		Statement stmt(pdb, sql_ss.str());
		stmt.bindInt(1, candidate.getEntryId());
		stmt.bindInt(2, candidate.getQuestionItemId());
		stmt.bindString(3, candidate.getItemValue());

		stmt.step();

		auto last_inserted_id = ::sqlite3_last_insert_rowid(pdb);
		auto last_inserted = this->getById(last_inserted_id);
		if (last_inserted.size() != 1) throw DatabaseException(-1, "Failed to get last inserted item.");
		retValue.push_back(last_inserted.at(0));

		transaction.commit();
	}
    return retValue;
}
