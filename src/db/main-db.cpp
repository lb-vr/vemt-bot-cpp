#ifdef VEMT_DB_MAIN_DB_BUILD
#include <iostream>
#include "QuestionItemsTable.hpp"
#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"

int main(){
    std::string dbPath = "./develop.sqlite3";

    vemt::db::QuestionItemsTable questionItemsTable(dbPath);
    auto questions = questionItemsTable.getById(2);
    std::cerr << questions.toString() << std::endl;

    vemt::db::SubmissionsTable submissionsTable(dbPath);
    auto submission = submissionsTable.getById(1);
    std::cerr << submission.toString() << std::endl;

    vemt::db::EntriesTable entriesTable(dbPath);
    auto entry = entriesTable.getById(1);
    std::cerr << entry.toString() << std::endl;
}
#endif
