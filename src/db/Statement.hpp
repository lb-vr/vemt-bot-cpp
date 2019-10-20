#ifndef VEMT_DB_STATEMENT_INCLUDED
#define VEMT_DB_STATEMENT_INCLUDED
//#include "sqlite3.h"
//#include "BaseTable.hpp"
//#include "AnswerModel.hpp"

#include "type/Type.hpp"
#include "DatabaseException.hpp"
#include <string>
#include <unordered_map>
#include <memory>

struct sqlite3;
struct sqlite3_value;
struct sqlite3_stmt;

namespace vemt {
namespace db {

class BindException : public DatabaseException {
public:
	BindException(const int error_code, const int index, const type::ParamBase & value);
	BindException(const int error_code, const int index);
};
class FetchException : public DatabaseException {
public:
	using DatabaseException::DatabaseException;
};

class Statement {
public:

	class GeneralValue {
	public:
		GeneralValue(const sqlite3_value * value);
		GeneralValue(GeneralValue && move) noexcept;
		type::IntParam		getAsInt() const;
		type::BoolParam		getAsBool() const;
		type::DatetimeParam getAsDatetime() const;
		type::PhaseParam	getAsPhase() const;
		type::StringParam	getAsString() const;
		type::WstringParam	getAsWstring() const;

		operator type::IntParam() const;
		operator type::BoolParam() const;
		operator type::DatetimeParam() const;
		operator type::PhaseParam() const;
		operator type::StringParam() const;
		operator type::WstringParam() const;
	private:
		std::unique_ptr<sqlite3_value> value_;
	};

	Statement(sqlite3 * pdb, const std::string & sql);
	~Statement() noexcept;

	void bindInt		(const int index, const type::IntParam & value);
	void bindInt		(const std::string & target, const type::IntParam & value);
	void bindBool		(const int index, const type::BoolParam & value);
	void bindBool		(const std::string & target, const type::BoolParam & value);
	void bindNull		(const int index);
	void bindNull		(const std::string & target);
	void bindString		(const int index, const type::StringParam & value);
	void bindString		(const std::string & target, const type::StringParam & value);
	void bindWstring	(const int index, const type::WstringParam & value);
	void bindWstring	(const std::string & target, const type::WstringParam & value);
	void bindDatetime	(const int index, const type::DatetimeParam & value);
	void bindDatetime	(const std::string & target, const type::DatetimeParam & value);

	void step();

	std::unordered_map<std::string, GeneralValue> fetch();

	void reset();

private:
	sqlite3_stmt * stmt_;
	int latest_code_;
};
} // namespace db
} // namespace vemt
#endif
