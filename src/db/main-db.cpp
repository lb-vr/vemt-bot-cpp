#include <iostream>
#include "QuestionItemsTable.hpp"
#include "QuestionItemModel.hpp"

int main(){
    std::string dbPath = "./develop.sqlite3";
    vemt::db::QuestionItemsTable QuestionItemsTable(dbPath);
    auto q = QuestionItemsTable.getById(1);
    std::cerr << q.toString() << std::endl;
}
