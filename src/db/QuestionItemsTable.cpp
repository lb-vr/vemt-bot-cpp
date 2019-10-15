#include "QuestionItemsTable.hpp"

std::string vemt::db::QuestionItemsTable::getTableName(){
    return std::string("question_items");
}

std::string vemt::db::QuestionItemsTable::getChoicesTableName(){
    return std::string("question_choices");
}

vemt::db::QuestionItemsTable::QuestionItemsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::type::WstringParam> vemt::db::QuestionItemsTable::getChoices(const int id){
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::type::WstringParam> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "C.title AS title "
            <<  "FROM " << vemt::db::QuestionItemsTable::getChoicesTableName() << " AS C "
            <<  "WHERE C.question_item_id=? "
            ;
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        while (::sqlite3_step(stmt) == SQLITE_ROW) {
			auto title = vemt::type::WstringParam();
			title.setAsCStr(sqlite3_column_text(stmt, 0), sqlite3_column_bytes(stmt, 0));
            retValue.push_back(title);
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}

vemt::db::QuestionItemModel vemt::db::QuestionItemsTable::getById(const int id)
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::QuestionItemModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "Q.id AS id, "
            <<  "Q.title AS title, "
            <<  "Q.detail AS detail, "
            <<  "Q.valid_type AS valid_type, "
            <<  "Q.regex AS regex, "
            <<  "Q.max_length AS max_length, "
            <<  "Q.required_when_phase AS required_when_phase, "
            <<  "Q.required_when_timepoint AS required_when_timepoint, "
            <<  "Q.allow_multiline AS allow_multiline, "
            <<  "Q.is_required AS is_required, "
            <<  "Q.created_at AS created_at, "
            <<  "COUNT(C.id) AS choices "
            <<  "FROM " << vemt::db::QuestionItemsTable::getTableName() << " AS Q "
            <<  "OUTER LEFT JOIN " << vemt::db::QuestionItemsTable::getChoicesTableName() << " AS C "
            <<  "ON C.question_item_id = Q.id "
            <<  "where Q.id=? "
            <<  "GROUP BY Q.id "
            <<  "LIMIT 1";
    try{
        stmt = this->prepareStatement(sql_ss.str());
        auto err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_ROW) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        auto _id = vemt::type::IntParam(sqlite3_column_int(stmt, 0));
		auto _title = vemt::type::WstringParam();
		_title.setAsCStr(sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));
		auto _detail = vemt::type::WstringParam();
		_detail.setAsCStr(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
		auto _valid_type = vemt::type::AnswerTypeParam();
		_valid_type.setAsInt(sqlite3_column_int(stmt, 3));
		auto _regex = vemt::type::WstringParam();
		_regex.setAsCStr(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
        auto _max_length = vemt::type::IntParam(sqlite3_column_int(stmt, 5));
        auto _required_when_phase = vemt::type::IntParam(sqlite3_column_int(stmt, 6));
		auto __required_when_timepoint = vemt::type::StringParam();
		__required_when_timepoint.setAsCStr(sqlite3_column_text(stmt, 7), sqlite3_column_bytes(stmt, 7));
		auto _required_when_timepoint = vemt::type::DatetimeParam();
		_required_when_timepoint.setAsString(__required_when_timepoint.get());
        auto _allow_multiline = vemt::type::BoolParam(sqlite3_column_int(stmt, 8));
        auto _is_required = vemt::type::BoolParam(sqlite3_column_int(stmt, 9));
		auto __created_at = vemt::type::StringParam();
		__created_at.setAsCStr(sqlite3_column_text(stmt, 10), sqlite3_column_bytes(stmt, 10));
		auto _created_at = vemt::type::DatetimeParam();
		_created_at.setAsString(__created_at.get());
        int n_choice = sqlite3_column_int(stmt, 11);
        std::vector<vemt::type::WstringParam> choices;
        if(n_choice != 0){
            choices = this->getChoices(_id.get());
        }
        retValue.push_back(QuestionItemModel(
            _id,
            _title,
            _detail,
            _valid_type,
            _regex,
            choices,
            _max_length,
            _is_required,
            _required_when_phase,
            _required_when_timepoint,
            _allow_multiline,
            _created_at
        ));
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue.at(0);
}

std::vector<vemt::db::QuestionItemModel> vemt::db::QuestionItemsTable::getAll()
{
    ::sqlite3_stmt *stmt = NULL;
    std::vector<vemt::db::QuestionItemModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "Q.id AS id "
            <<  "FROM " << vemt::db::QuestionItemsTable::getTableName() << " AS Q "
            ;
    try{
        stmt = this->prepareStatement(sql_ss.str());
        while (::sqlite3_step(stmt) == SQLITE_ROW) {
            auto id = sqlite3_column_int(stmt, 0);
            retValue.push_back(
                this->getById(id)
            );
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}
