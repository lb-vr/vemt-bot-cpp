#ifndef VEMT_DB_DATABASEEXCEPTION_INCLUDED
#define VEMT_DB_DATABASEEXCEPTION_INCLUDED
#include <stdexcept>
namespace vemt
{
namespace db
{

class DatabaseException : public std::runtime_error {
public:
	DatabaseException(const int error_code, const char * message = "")
		: std::runtime_error(message), error_code_(error_code) {}
private:
	const int error_code_;
};

} // namespace db
} // namespace vemt
#endif
