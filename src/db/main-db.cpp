#ifdef VEMT_DB_MAIN_DB_BUILD
#include <iostream>
#include "QuestionItemsTable.hpp"
#include "SubmissionsTable.hpp"
#include "EntriesTable.hpp"
#include "AnswersTable.hpp"

int main(){
    std::string dbPath = "./develop.sqlite3";

    vemt::db::EntriesTable          entriesTable(dbPath);
    vemt::db::QuestionItemsTable    questionItemsTable(dbPath);
    vemt::db::SubmissionsTable      submissionsTable(dbPath);
    vemt::db::AnswersTable          answersTable(dbPath);

    auto entries = entriesTable.getById(1);
    std::cerr << "entry(id=1):\t";
    for(auto e : entries){
        std::cerr << e.toString() << std::endl;
    }

    vemt::db::EntryModel tmp(
        vemt::type::IntParam(300),
        vemt::type::PhaseParam(vemt::type::Phase::kSubmit),
        vemt::type::IntParam(301),
        vemt::type::IntParam(302)
    );
    auto inserted_s = entriesTable.insert(tmp);
    for(auto i : inserted_s){
        std::cerr << "Inserted:\t";
        std::cerr << i.toString() << std::endl;
    }

    auto questions = questionItemsTable.getAll();
    for(auto q : questions){
        std::cerr << "Questions:\t";
        std::cerr << q.toString() << std::endl;
    }

    auto submissions = submissionsTable.getById(1);
    for(auto s:submissions){
        std::cerr << s.toString() << std::endl;
    }

    auto answers = answersTable.getByDiscordUserId(entries.at(0).getDiscordUid());
    for(auto a : answers){
        std::cerr << a.toString() << std::endl;
    }

}
#endif
