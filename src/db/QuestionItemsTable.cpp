#include "QuestionItemsTable.hpp"
#include "Statement.hpp"
#include "Transaction.hpp"
#include <sstream>

std::string vemt::db::QuestionItemsTable::getTableName(){
    return std::string("question_items");
}

std::string vemt::db::QuestionItemsTable::getChoicesTableName(){
    return std::string("question_choices");
}

vemt::db::QuestionItemsTable::QuestionItemsTable(const std::string & dbPath) noexcept: BaseTable(dbPath){
}

std::vector<vemt::type::WstringParam> vemt::db::QuestionItemsTable::getChoices(const int id){
    std::vector<vemt::type::WstringParam> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
            <<  "C.title AS title "
            <<  "FROM " << vemt::db::QuestionItemsTable::getChoicesTableName() << " AS C "
            <<  "WHERE C.question_item_id=? "
            ;

    Statement stmt(this->pdb, sql_ss.str());
	stmt.bindInt(1, id);
    while (stmt.step()) {
		auto fetched_value = stmt.fetch();
		retValue.push_back(
			fetched_value.at("title").getAsWstring()
		);
	}
    return retValue;
}

std::vector<vemt::db::QuestionItemModel> vemt::db::QuestionItemsTable::getById(const int id)
{
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
            <<  "Q.created_at AS created_at "
            <<  "FROM " << vemt::db::QuestionItemsTable::getTableName() << " AS Q "
            <<  "OUTER LEFT JOIN " << vemt::db::QuestionItemsTable::getChoicesTableName() << " AS C "
            <<  "ON C.question_item_id = Q.id "
            <<  "where Q.id=? "
            <<  "GROUP BY Q.id "
            <<  "LIMIT 1";

        Statement stmt(this->pdb, sql_ss.str());
        stmt.bindInt(1, id);
        
        while (stmt.step()) {
            auto fetched_value = stmt.fetch();
            retValue.push_back(QuestionItemModel(
                fetched_value.at("id").getAsInt(),
                fetched_value.at("title").getAsWstring(),
                fetched_value.at("detail").getAsWstring(),
                fetched_value.at("valid_type").getAsAnswerType(),
                fetched_value.at("regex").getAsWstring(),
                this->getChoices(fetched_value.at("id").getAsInt().get()),
                fetched_value.at("max_length").getAsInt(),
                fetched_value.at("is_required").getAsBool(),
                fetched_value.at("required_when_phase").getAsInt(),
                fetched_value.at("required_when_timepoint").getAsDatetime(),
                fetched_value.at("allow_multiline").getAsBool(),
                fetched_value.at("created_at").getAsDatetime()
            ));
    	}
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
    std::stringstream sql_delete, sql_insert, sql_inssub;
    sql_delete
        << "DELETE FROM " << vemt::db::QuestionItemsTable::getTableName()
        ;
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

        err = sqlite3_exec(pdb, sql_delete.str().c_str(), nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << __FILE__ << " : " << __LINE__ << "CANNOT DROP current DATA" << std::endl;
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
