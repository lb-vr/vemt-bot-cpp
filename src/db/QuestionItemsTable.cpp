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
                this->getChoices(fetched_value.at("id").getAsInt()),
                fetched_value.at("max_length").getAsInt(),
                fetched_value.at("is_required").getAsBool(),
                fetched_value.at("required_when_phase").getAsPhase(),
                fetched_value.at("required_when_timepoint").getAsDatetime(),
                fetched_value.at("allow_multiline").getAsBool(),
                fetched_value.at("created_at").getAsDatetime()
            ));
    	}
    return retValue;
}

std::vector<vemt::db::QuestionItemModel> vemt::db::QuestionItemsTable::getAll()
{
    std::vector<vemt::db::QuestionItemModel> retValue;
    std::stringstream sql_ss;
    sql_ss  <<  "SELECT "
        <<  "Q.id AS id "
        <<  "FROM " << vemt::db::QuestionItemsTable::getTableName() << " AS Q "
        ;

   Statement stmt(this->pdb, sql_ss.str());
   while (stmt.step()) {
       auto fetched_value = stmt.fetch();
       retValue.push_back(
           this->getById(fetched_value.at("id").getAsInt()).at(0)
       );
    }
    return retValue;
}

std::vector<vemt::db::QuestionItemModel> vemt::db::QuestionItemsTable::replaceAll(std::vector<vemt::db::QuestionItemModel> values)
{
    std::vector<vemt::db::QuestionItemModel> retValue;
    std::stringstream sql_delete, sql_delete_choices, sql_insert, sql_inssub;
	sql_delete_choices
		<< "DELETE FROM " << vemt::db::QuestionItemsTable::getChoicesTableName()
		;
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
        <<  ") VALUES (:title, :detail, :valid_type, :regex, :max_length, :req_phase, :req_time, :multiline, :required)"
        ;
    sql_inssub
        <<  "INSERT "
        <<  "INTO " << vemt::db::QuestionItemsTable::getChoicesTableName() << " ("
        <<  "question_item_id, "
        <<  "title"
        <<  ") VALUES (:parent_id, :title)"
        ;

	Statement stmt_delete_choices(this->pdb, sql_delete_choices.str());
    Statement stmt_delete(this->pdb, sql_delete.str());
    Statement stmt_insert(this->pdb, sql_insert.str());
    Statement stmt_inssub(this->pdb, sql_inssub.str());

    vemt::db::Transaction locking(this->pdb);
	stmt_delete_choices.step();
    stmt_delete.step();
    for(auto v : values){
        stmt_insert.reset();
        //stmt_insert.clearbinding();
        
        stmt_insert.bindString(":title", v.getText().toString());
        stmt_insert.bindString(":detail", v.getDetailText().toString());
        stmt_insert.bindInt   (":valid_type", v.getType().getAsInt());
        stmt_insert.bindString(":regex", v.getRegexRule().toString());
        stmt_insert.bindInt   (":max_length", v.getLength());
        stmt_insert.bindPhase (":req_phase", v.getRequiredWhenPhase());
        stmt_insert.bindString(":req_time", v.getRequireWhenDatetime().toString());
        stmt_insert.bindBool  (":multiline", v.getMultiline());
        stmt_insert.bindBool  (":required", v.getIsRequired());

        stmt_insert.step();
        auto last_inserted_id = sqlite3_last_insert_rowid(pdb);

        for(auto c : v.getChoise()){
            stmt_inssub.reset();
            //stmt_inssub.clearbinding();

            stmt_inssub.bindInt   (":parent_id", last_inserted_id);
            stmt_inssub.bindString(":title", c.toString());
            stmt_inssub.step();
        }
    }

    locking.commit();

    return this->getAll();
}
