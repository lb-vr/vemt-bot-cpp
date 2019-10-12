#ifdef VEMT_DB_MAIN_DB_BUILD
#include <iostream>
#include "QuestionItemsTable.hpp"
#include "QuestionItemModel.hpp"

int main(){
    std::string dbPath = "./develop.sqlite3";
    vemt::db::QuestionItemsTable QuestionItemsTable(dbPath);
    auto questions = QuestionItemsTable.getAll();
    for(auto q : questions){
        std::cerr << q.toString() << std::endl;
    }
}
#endif
