#include "AnswersTable.hpp"
#include "EntriesTable.hpp"
std::string vemt::db::AnswersTable::getTableName(){
    return std::string("answers");
}

vemt::db::AnswersTable::AnswersTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::AnswerModel> vemt::db::AnswersTable::getByDiscordUserId(const int discord_user_id)
{
    ::sqlite3_stmt *stmt = NULL;
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
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, discord_user_id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        while ((err = ::sqlite3_step(stmt)) == SQLITE_ROW) {
            auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
            auto _entry_id = vemt::type::IntParam(sqlite3_column_int(stmt, 1));
            auto _question_item_id = vemt::type::IntParam(sqlite3_column_int(stmt, 2));

			auto _item_value = vemt::type::StringParam();
			_item_value.setAsCStr(sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

			auto __created_at = vemt::type::StringParam();
			__created_at.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));

			auto _created_at = vemt::type::DatetimeParam();
			_created_at.setAsString(__created_at.get(), "%Y-%m-%d %H:%M:%S");

			auto __updated_at = vemt::type::StringParam();
			__updated_at.setAsCStr(sqlite3_column_text(stmt, 5), sqlite3_column_bytes(stmt, 5));

			auto _updated_at = vemt::type::DatetimeParam();
			_updated_at.setAsString(__updated_at.get(), "%Y-%m-%d %H:%M:%S");

            auto a = AnswerModel(
                    _id,
                    _entry_id,
                    _question_item_id,
                    _item_value,
                    _created_at,
                    _updated_at
                );
            std::cerr << "\t\t" <<a.getCreatedAt() << std::endl;
            retValue.push_back(
                a
            );
        }
        if (err != SQLITE_DONE) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    return retValue;
}
