#include "QuestionItemsTable.hpp"

std::string vemt::db::QuestionItemsTable::getTableName(){
    return std::string("question_items");
}

std::string vemt::db::QuestionItemsTable::getChoicesTableName(){
    return std::string("question_choices");
}

vemt::db::QuestionItemsTable::QuestionItemsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::db::type::StringParam> vemt::db::QuestionItemsTable::getChoices(const int id){
    std::vector<vemt::db::type::StringParam> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "C.title AS title "
            <<  "FROM " << vemt::db::QuestionItemsTable::getChoicesTableName() << " AS C "
            <<  "where C.question_item_id=? "
            ;
    try{
        auto err = this->prepareStatement(sql_ss.str());
        if (err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
            throw std::exception();
        }
        err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        while (::sqlite3_step(stmt) == SQLITE_ROW) {
            auto title = vemt::db::type::StringParam(sqlite3_column_text(stmt, 0), sqlite3_column_bytes(stmt, 0));
            retValue.push_back(title);
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    return retValue;
}

vemt::db::QuestionItemModel vemt::db::QuestionItemsTable::getById(const int id)
{
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
            <<  "STRFTIME('%s', Q.created_at) AS created_at, "
            <<  "COUNT(C.id) AS choices "
            <<  "FROM " << vemt::db::QuestionItemsTable::getTableName() << " AS Q "
            <<  "OUTER LEFT JOIN " << vemt::db::QuestionItemsTable::getChoicesTableName() << " AS C "
            <<  "ON C.question_item_id = Q.id "
            <<  "where Q.id=? "
            <<  "GROUP BY Q.id "
            <<  "LIMIT 1";
    try{
        auto err = this->prepareStatement(sql_ss.str());
        if (err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "; err=" << err << std::endl;
            throw std::exception();
        }
        err = ::sqlite3_bind_int(stmt, 1, id);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_step(stmt);
        if (err != SQLITE_ROW) {
            std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
            throw std::exception();
        }
        auto _id = vemt::db::type::IntParam(sqlite3_column_int(stmt, 0));
        auto _title = vemt::db::type::StringParam(sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));
        auto _detail = vemt::db::type::StringParam(sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));
        auto _valid_type = vemt::db::type::IntParam(sqlite3_column_int(stmt, 3));
        auto _regex = vemt::db::type::StringParam(sqlite3_column_text(stmt, 4), sqlite3_column_bytes(stmt, 4));
        auto _max_length = vemt::db::type::IntParam(sqlite3_column_int(stmt, 5));
        auto _required_when_phase = vemt::db::type::IntParam(sqlite3_column_int(stmt, 6));
        auto _required_when_timepoint = sqlite3_column_int(stmt, 7);
        auto _allow_multiline = vemt::db::type::BoolParam(sqlite3_column_int(stmt, 8));
        auto _is_required = vemt::db::type::BoolParam(sqlite3_column_int(stmt, 9));
        auto _created_at = sqlite3_column_int(stmt, 10);
        int n_choice = sqlite3_column_int(stmt, 11);
        std::vector<vemt::db::type::StringParam> choices;
        if(n_choice != 0){
            choices = this->getChoices(_id.get());
        }
        return QuestionItemModel(
            _id,
            _title,
            _detail,
            _valid_type,
            _regex,
            choices,
            _max_length,
            _is_required,
            _required_when_phase,
            vemt::db::type::DatetimeParam(std::chrono::system_clock::from_time_t(_required_when_timepoint)),
            _allow_multiline,
            vemt::db::type::DatetimeParam(std::chrono::system_clock::from_time_t(_created_at))
        );
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
}
