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
