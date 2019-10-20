#include "Transaction.hpp"
#include "sqlite3.h"
#include "DatabaseException.hpp"
#include <mutex>

namespace {
std::mutex mtx_;
}

vemt::db::Transaction::Transaction(sqlite3 * const pdb) : pdb_(pdb), lock_(mtx_) {
	auto ret = ::sqlite3_exec(this->pdb_, "BEGIN;", nullptr, nullptr, nullptr);
	if (ret != SQLITE_OK) throw DatabaseException(ret, "Failed to begin transaction.");
}

vemt::db::Transaction::~Transaction() {
	if (this->pdb_ != nullptr) {
		auto ret = sqlite3_exec(this->pdb_, "ROLLBACK;", nullptr, nullptr, nullptr);
		if (ret != SQLITE_OK) {
			abort();
		}
		this->pdb_ = nullptr;
	}
}

void vemt::db::Transaction::commit() {
	if (this->pdb_ != nullptr) {
		auto ret = sqlite3_exec(this->pdb_, "COMMIT;", nullptr, nullptr, nullptr);
		if (ret != SQLITE_OK) throw DatabaseException(ret, "Failed to commit transaction.");
		this->pdb_ = nullptr;
	}
}

