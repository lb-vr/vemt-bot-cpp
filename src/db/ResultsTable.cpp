#include "ResultsTable.hpp"
#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"
#include "Statement.hpp"
std::string vemt::db::ResultsTable::getTableName(){
    return std::string("results");
}

vemt::db::ResultsTable::ResultsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::ResultModel> vemt::db::ResultsTable::getById(const int id)
{
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
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, id);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.emplace_back(
			fetched_value.at("id").getAsInt(),
			fetched_value.at("submission_id").getAsInt(),
			fetched_value.at("as_phase").getAsInt(),
			fetched_value.at("is_passed").getAsBool(),
			fetched_value.at("log_text").getAsString(),
			fetched_value.at("created_at").getAsDatetime()
		);
	}
    return retValue;
}

std::vector<vemt::db::ResultModel> vemt::db::ResultsTable::getBySubmissionId(const int submission_id)
{
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
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, submission_id);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.emplace_back(
			fetched_value.at("id").getAsInt(),
			fetched_value.at("submission_id").getAsInt(),
			fetched_value.at("as_phase").getAsInt(),
			fetched_value.at("is_passed").getAsBool(),
			fetched_value.at("log_text").getAsString(),
			fetched_value.at("created_at").getAsDatetime()
		);
	}
    return retValue;
}

std::vector<vemt::db::ResultModel> vemt::db::ResultsTable::getByDiscordUserId(const int discord_user_id)
{
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
            <<  "ON S.entry_id = E.id "
            <<  "WHERE E.discord_user_id=? "
            ;
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, discord_user_id);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.emplace_back(
			fetched_value.at("id").getAsInt(),
			fetched_value.at("submission_id").getAsInt(),
			fetched_value.at("as_phase").getAsInt(),
			fetched_value.at("is_passed").getAsBool(),
			fetched_value.at("log_text").getAsString(),
			fetched_value.at("created_at").getAsDatetime()
		);
	}
    return retValue;
}
