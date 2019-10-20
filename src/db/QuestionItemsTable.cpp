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

std::vector<vemt::db::QuestionItemModel> vemt::db::QuestionItemsTable::getById(const int id)
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
    return retValue;
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
			auto ret = this->getById(id);
			if (ret.size() == 1) {
				retValue.push_back(ret.at(0));
			}
			else {
				assert(ret.size() == 1);
			}
        }
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }
    this->finalizeStatement(stmt);
    return retValue;
}

std::vector<vemt::db::QuestionItemModel> vemt::db::QuestionItemsTable::replaceAll(std::vector<vemt::db::QuestionItemModel> values)
{
    ::sqlite3_stmt *stmt_insert = NULL, *stmt_inssub = NULL;
    std::stringstream sql_insert, sql_inssub;
    sql_insert
        <<  "INSERT "
        <<  "INTO " << vemt::db::QuestionItemsTable::getTableName() << " ("
        <<  "title, "
        <<  "detail, "
        <<  "valid_type, "
        <<  "regex, "
        <<  "max_length, "
        <<  "required_when_phase, "
        <<  "required_when_timepoint, "
        <<  "allow_multiline, "
        <<  "is_required"
        <<  ") VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
        ;
    sql_inssub
        <<  "INSERT "
        <<  "INTO " << vemt::db::QuestionItemsTable::getChoicesTableName() << " ("
        <<  "question_item_id, "
        <<  "title"
        <<  ") VALUES (?, ?)"
        ;
    auto err = sqlite3_exec(pdb, "BEGIN;", nullptr, nullptr, nullptr);
    try{
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "CANNOT BEGIN TRANSACTION" << std::endl;
            throw std::exception();
        }
        stmt_insert = this->prepareStatement(sql_insert.str());
        stmt_inssub = this->prepareStatement(sql_inssub.str());

        for(auto v : values){
            err  = ::sqlite3_reset(stmt_insert);
            err |= ::sqlite3_clear_bindings(stmt_insert);
            if(err != SQLITE_OK){
                std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
                throw std::exception();
            }
			auto text = v.getText().toString();
			auto detail_text = v.getDetailText().toString();
			auto regex_rule = v.getRegexRule().toString();
			auto require_when_datetime_str = v.getRequireWhenDatetime().toString();
            err  = ::sqlite3_bind_text(stmt_insert, 1, text.c_str(), text.length(), NULL);
            //err  = ::sqlite3_bind_text(stmt_insert, 1, v.getText().toString().c_str(), v.getText().toString().length(), NULL);
            err |= ::sqlite3_bind_text(stmt_insert, 2, detail_text.c_str(), detail_text.length(), NULL);
            //err |= ::sqlite3_bind_text(stmt_insert, 2, v.getDetailText().toString().c_str(), v.getDetailText().toString().length(), NULL);
            err |= ::sqlite3_bind_int (stmt_insert, 3, v.getType().getAsInt());
            err |= ::sqlite3_bind_text(stmt_insert, 4, regex_rule.c_str(), regex_rule.length(), NULL);
            //err |= ::sqlite3_bind_text(stmt_insert, 4, v.getRegexRule().toString().c_str(), v.getRegexRule().toString().length(), NULL);
            err |= ::sqlite3_bind_int (stmt_insert, 5, v.getLength().get());
            err |= ::sqlite3_bind_int (stmt_insert, 6, v.getRequiredWhenPhase().get());
            err |= ::sqlite3_bind_text(stmt_insert, 7, require_when_datetime_str.c_str(), require_when_datetime_str.length(), NULL);
            //err |= ::sqlite3_bind_text(stmt_insert, 7, v.getRequireWhenDatetime().getAsString().c_str(), v.getRequireWhenDatetime().getAsString().length(), NULL);
            err |= ::sqlite3_bind_int (stmt_insert, 8, v.getMultiline().get());
            err |= ::sqlite3_bind_int (stmt_insert, 9, v.getIsRequired().get());
            if(err != SQLITE_OK){
                std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
                throw std::exception();
            }
            err = ::sqlite3_step(stmt_insert);
            if (err != SQLITE_DONE) {
                std::cerr << __FILE__ << " : " << __LINE__ << "\t" << err << std::endl;
                throw std::exception();
            }
            auto last_inserted_id = sqlite3_last_insert_rowid(pdb);
            for(auto c : v.getChoise()){
                err  = ::sqlite3_reset(stmt_inssub);
                err |= ::sqlite3_clear_bindings(stmt_inssub);
                if(err != SQLITE_OK){
                    std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
                    throw std::exception();
                }
				auto choise_text = c.toString();
                err  = ::sqlite3_bind_int (stmt_inssub, 1, last_inserted_id);
                err |= ::sqlite3_bind_text(stmt_inssub, 2, choise_text.c_str(), choise_text.length(), NULL);
                // err |= ::sqlite3_bind_text(stmt_inssub, 2, c.toString().c_str(), c.toString().length(), NULL);
                if(err != SQLITE_OK){
                    std::cerr << __FILE__ << " : " << __LINE__ << std::endl;
                    throw std::exception();
                }
                err = ::sqlite3_step(stmt_inssub);
                if (err != SQLITE_DONE) {
                    std::cerr << __FILE__ << " : " << __LINE__ << "\t" << err << std::endl;
                    throw std::exception();
                }
            }
        }
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
    this->finalizeStatement(stmt_inssub);
    this->finalizeStatement(stmt_insert);
    return this->getAll();
}
