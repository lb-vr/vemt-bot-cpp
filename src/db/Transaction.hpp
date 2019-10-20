#ifndef VEMT_DB_TRANSACTION_HPP
#define VEMT_DB_TRANSACTION_HPP

#include <mutex>

struct sqlite3;

namespace vemt {
namespace db {

class Transaction {
public:
	Transaction(sqlite3 *const pdb);
	~Transaction();
	void commit();

private:
	sqlite3 * pdb_;
	std::lock_guard<std::mutex> lock_;
};

}
}

#endif
