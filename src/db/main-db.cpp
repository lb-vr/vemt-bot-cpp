#ifdef VEMT_DB_MAIN_DB_BUILD
#include <iostream>
#include "QuestionItemsTable.hpp"
#include "SubmissionsTable.hpp"

int main(){
    std::string dbPath = "./develop.sqlite3";
    vemt::db::QuestionItemsTable questionItemsTable(dbPath);
    auto questions = questionItemsTable.getById(1);
    std::cerr << questions.toString() << std::endl;
    /*for(auto q : questions){
        std::cerr << q.toString() << std::endl;
    }*/
    vemt::db::SubmissionsTable submissionsTable(dbPath);
    auto submission = submissionsTable.getById(1);
    std::cerr << submission.toString() << std::endl;
}
#endif
