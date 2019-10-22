#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"
#include "Statement.hpp"

std::string vemt::db::SubmissionsTable::getTableName(){
    return std::string("submissions");
}

vemt::db::SubmissionsTable::SubmissionsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::SubmissionModel> vemt::db::SubmissionsTable::getById(const long int id)
{
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
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, id);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.emplace_back(
			fetched_value.at("id").getAsInt(),
			fetched_value.at("entry_id").getAsInt(),
			fetched_value.at("package_url").getAsString(),
			fetched_value.at("current_phase").getAsInt(),
			fetched_value.at("created_at").getAsDatetime(),
            fetched_value.at("updated_at").getAsDatetime()
		);
	}
    return retValue;
}

std::vector<vemt::db::SubmissionModel> vemt::db::SubmissionsTable::getByDiscordUid(const int64_t id)
{
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
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, id);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.emplace_back(
			fetched_value.at("id").getAsInt(),
			fetched_value.at("entry_id").getAsInt(),
			fetched_value.at("package_url").getAsString(),
			fetched_value.at("current_phase").getAsInt(),
			fetched_value.at("created_at").getAsDatetime(),
            fetched_value.at("updated_at").getAsDatetime()
		);
	}
    return retValue;
}
