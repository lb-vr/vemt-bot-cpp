#ifdef VEMT_DB_MAIN_DB_BUILD
#include <iostream>
#include "QuestionItemsTable.hpp"
#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"
#include "AnswersTable.hpp"

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
    //auto inserted = 
    vemt::db::EntryModel tmp(
        //vemt::type::IntParam(3),
        vemt::type::IntParam(300),
        vemt::type::IntParam(2),
        vemt::type::IntParam(301),
        vemt::type::IntParam(302)//,
        //vemt::type::DatetimeParam(1500),
        //vemt::type::DatetimeParam(46464)
    );
    auto inserted = entriesTable.insert(tmp);
    std::cerr << inserted.toString() << std::endl;

    vemt::db::AnswersTable answersTable(dbPath);
    auto answers = answersTable.getByDiscordUserId(entry.getDiscordUid());
    for(auto a : answers){
        std::cerr << a.toString() << std::endl;
    }

}
#endif
