#include "RegistryTable.hpp"
#include "Statement.hpp"
#include "Transaction.hpp"

std::string vemt::db::RegistryTable::getIntTableName(){
    return std::string("registry_int");
}
std::string vemt::db::RegistryTable::getBoolTableName(){
    return std::string("registry_int");
}
std::string vemt::db::RegistryTable::getStringTableName(){
    return std::string("registry_string");
}
std::string vemt::db::RegistryTable::getWstringTableName(){
    return std::string("registry_string");
}
std::string vemt::db::RegistryTable::getDatetimeTableName(){
    return std::string("registry_datetime");
}

vemt::db::RegistryTable::RegistryTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

const vemt::type::IntParam & vemt::db::RegistryTable::getInt(const std::string & key)
{
    vemt::type::IntParam retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            //<<  "R.id AS id, "
            //<<  "R.title AS title, "
            <<  "R.itemvalue AS itemvalue "
            //<<  "R.created_at AS created_at, "
            //<<  "R.updated_at AS updated_at "
            <<  "FROM " << vemt::db::RegistryTable::getIntTableName() << " AS R "
            <<  "WHERE title=? "
            <<  "LIMIT 1";
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindString(1, key);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue = fetched_value.at("itemvalue").getAsInt();
	}
    return retValue;
}

const vemt::type::BoolParam & vemt::db::RegistryTable::getBool(const std::string & key)
{
    vemt::type::BoolParam retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            //<<  "R.id AS id, "
            //<<  "R.title AS title, "
            <<  "R.itemvalue AS itemvalue "
            //<<  "R.created_at AS created_at, "
            //<<  "R.updated_at AS updated_at "
            <<  "FROM " << vemt::db::RegistryTable::getBoolTableName() << " AS R "
            <<  "WHERE title=? "
            <<  "LIMIT 1";
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindString(1, key);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue = fetched_value.at("itemvalue").getAsBool();
	}
    return retValue;
}

const vemt::type::StringParam & vemt::db::RegistryTable::getString(const std::string & key)
{
    vemt::type::StringParam retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            //<<  "R.id AS id, "
            //<<  "R.title AS title, "
            <<  "R.itemvalue AS itemvalue "
            //<<  "R.created_at AS created_at, "
            //<<  "R.updated_at AS updated_at "
            <<  "FROM " << vemt::db::RegistryTable::getStringTableName() << " AS R "
            <<  "WHERE title=? "
            <<  "LIMIT 1";
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindString(1, key);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue = fetched_value.at("itemvalue").getAsString();
	}
    return retValue;
}

const vemt::type::WstringParam & vemt::db::RegistryTable::getWstring(const std::string & key)
{
    vemt::type::WstringParam retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            //<<  "R.id AS id, "
            //<<  "R.title AS title, "
            <<  "R.itemvalue AS itemvalue "
            //<<  "R.created_at AS created_at, "
            //<<  "R.updated_at AS updated_at "
            <<  "FROM " << vemt::db::RegistryTable::getStringTableName() << " AS R "
            <<  "WHERE title=? "
            <<  "LIMIT 1";
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindString(1, key);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue = fetched_value.at("itemvalue").getAsWstring();
	}
    return retValue;
}

const vemt::type::DatetimeParam & vemt::db::RegistryTable::getDatetime(const std::string & key)
{
    vemt::type::DatetimeParam retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            //<<  "R.id AS id, "
            //<<  "R.title AS title, "
            <<  "R.itemvalue AS itemvalue "
            //<<  "R.created_at AS created_at, "
            //<<  "R.updated_at AS updated_at "
            <<  "FROM " << vemt::db::RegistryTable::getDatetimeTableName() << " AS R "
            <<  "WHERE title=? "
            <<  "LIMIT 1";
    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindString(1, key);

	while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue = fetched_value.at("itemvalue").getAsDatetime();
	}
    return retValue;
}

void vemt::db::RegistryTable::setInt     (const std::string & key, vemt::type::IntParam      & value)
{
    std::stringstream sql_insert;
    sql_insert
        <<  "REPLACE "
        <<  "INTO " << vemt::db::RegistryTable::getIntTableName() << " ("
        <<  "key, "
        <<  "itemvalue "
        <<  ") VALUES (:key, :itemvalue)"
        ;

    Statement stmt_insert(this->pdb, sql_insert.str());
    vemt::db::Transaction locking(this->pdb);
        stmt_insert.bindString(":key", key);
        stmt_insert.bindInt(":itemvalue", value);

        stmt_insert.step();
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
    locking.commit();
    return;
}

void vemt::db::RegistryTable::setBool    (const std::string & key, vemt::type::BoolParam     & value)
{
    std::stringstream sql_insert;
    sql_insert
        <<  "REPLACE "
        <<  "INTO " << vemt::db::RegistryTable::getIntTableName() << " ("
        <<  "key, "
        <<  "itemvalue "
        <<  ") VALUES (:key, :itemvalue)"
        ;

    Statement stmt_insert(this->pdb, sql_insert.str());
    vemt::db::Transaction locking(this->pdb);
        stmt_insert.bindString(":key", key);
        stmt_insert.bindBool(":itemvalue", value);

        stmt_insert.step();
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
    locking.commit();
    return;
}

void vemt::db::RegistryTable::setString  (const std::string & key, vemt::type::StringParam   & value)
{
    std::stringstream sql_insert;
    sql_insert
        <<  "REPLACE "
        <<  "INTO " << vemt::db::RegistryTable::getIntTableName() << " ("
        <<  "key, "
        <<  "itemvalue "
        <<  ") VALUES (:key, :itemvalue)"
        ;

    Statement stmt_insert(this->pdb, sql_insert.str());
    vemt::db::Transaction locking(this->pdb);
        stmt_insert.bindString(":key", key);
        stmt_insert.bindString(":itemvalue", value);

        stmt_insert.step();
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
    locking.commit();
    return;
}

void vemt::db::RegistryTable::setWstring (const std::string & key, vemt::type::WstringParam  & value)
{
    std::stringstream sql_insert;
    sql_insert
        <<  "REPLACE "
        <<  "INTO " << vemt::db::RegistryTable::getIntTableName() << " ("
        <<  "key, "
        <<  "itemvalue "
        <<  ") VALUES (:key, :itemvalue)"
        ;

    Statement stmt_insert(this->pdb, sql_insert.str());
    vemt::db::Transaction locking(this->pdb);
        stmt_insert.bindString(":key", key);
        stmt_insert.bindWstring(":itemvalue", value);

        stmt_insert.step();
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
    locking.commit();
    return;
}

void vemt::db::RegistryTable::setDatetime(const std::string & key, vemt::type::DatetimeParam & value)
{
    std::stringstream sql_insert;
    sql_insert
        <<  "REPLACE "
        <<  "INTO " << vemt::db::RegistryTable::getIntTableName() << " ("
        <<  "key, "
        <<  "itemvalue "
        <<  ") VALUES (:key, :itemvalue)"
        ;

    Statement stmt_insert(this->pdb, sql_insert.str());
    vemt::db::Transaction locking(this->pdb);
        stmt_insert.bindString(":key", key);
        stmt_insert.bindDatetime(":itemvalue", value);

        stmt_insert.step();
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
    locking.commit();
    return;
}
