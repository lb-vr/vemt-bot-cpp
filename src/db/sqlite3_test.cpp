#ifdef VEMT_DB_SQLITE3TEST_BUILD
#include <iostream>
#include <sstream>
#include <thread>
#include "sqlite3.h"

void threadTest(void){
    std::string dbPath = "./test.db";
    ::sqlite3 *pdb = nullptr;
    ::sqlite3_stmt *stmt = nullptr;
    std::string sql = "INSERT INTO test1(value) VALUES (?)";
    std::stringstream data;
    const auto tid = std::this_thread::get_id();

    auto err = ::sqlite3_open_v2(dbPath.c_str(), &pdb, SQLITE_OPEN_READWRITE|SQLITE_OPEN_FULLMUTEX, 0);
    if (err != SQLITE_OK){
        std::cerr << tid << " : " << __LINE__ << "; err=" << err << std::endl;
        throw std::exception();
    }
    err = ::sqlite3_prepare_v2(
        pdb,
        sql.c_str(),
        sql.length(),
        &stmt,
        nullptr
    );
    if(err != SQLITE_OK){
        std::cerr << tid << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
        throw std::exception();
    }
    for(size_t i=0; i<10000; i++){
        data.clear();
        data << "Thread<" << tid << "> " << i;
        err  = ::sqlite3_reset(stmt);
        if(err != SQLITE_OK){
            std::cerr << tid << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
            throw std::exception();
        }
        err |= ::sqlite3_clear_bindings(stmt);
        if(err != SQLITE_OK){
            std::cerr << tid << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
            throw std::exception();
        }

        err = ::sqlite3_bind_text(stmt, 1, data.str().c_str(), data.str().length(), nullptr);
        if(err != SQLITE_OK){
            std::cerr << tid << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
            throw std::exception();
        }

        //std::this_thread::yield();

        err = sqlite3_exec(pdb, "BEGIN;", nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << tid << " : " << __LINE__ << "; err=" << err << "; " << sql << std::endl;
            throw std::exception();
        }
        err = ::sqlite3_step(stmt);
        if (err != SQLITE_DONE) {
            std::cerr << tid << " : " << __LINE__ << "\t" << err << std::endl;
            throw std::exception();
        }
        err = sqlite3_exec(pdb, "COMMIT;", nullptr, nullptr, nullptr);
        if(err != SQLITE_OK){
            std::cerr << tid << " : " << __LINE__ << " CANNOT COMMIT TRANSACTION" << std::endl;
            throw std::exception();
        }
    }
    ::sqlite3_close_v2(pdb);
}

int main(){
    try{
        std::thread th1(threadTest);
        std::thread th2(threadTest);
        std::cerr << "t1: " << th1.get_id() << std::endl;
        std::cerr << "t2: " << th2.get_id() << std::endl;

        th1.join();
        th2.join();
    }catch (std::exception e){
        std::cerr << e.what() << std::endl;
    }

}

#endif