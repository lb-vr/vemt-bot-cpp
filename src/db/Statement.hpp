#ifndef VEMT_DB_STATEMENT_INCLUDED
#define VEMT_DB_STATEMENT_INCLUDED

#include "type/Type.hpp"
#include "DatabaseException.hpp"
#include <string>
#include <unordered_map>
#include <memory>

struct sqlite3;
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
		GeneralValue() noexcept;
		GeneralValue(const GeneralValue & copy) noexcept;
		GeneralValue(GeneralValue && move) noexcept;
		virtual ~GeneralValue();

		void setAsInt(const int64_t value);
		void setAsReal(const double value);
		void setAsString(const unsigned char * const text, const int bytes);
		void setAsBlob(const void * const blob, const int bytes);

		type::IntParam		getAsInt() const;
		type::BoolParam		getAsBool() const;
		type::DatetimeParam getAsDatetime() const;
		type::PhaseParam	getAsPhase() const;
		type::StringParam	getAsString() const;
		type::WstringParam	getAsWstring() const;
		std::vector<unsigned char>	getAsBlob() const;

		GeneralValue & operator=(const GeneralValue & v);

		operator vemt::type::IntParam() const;
		operator vemt::type::BoolParam() const;
		operator vemt::type::DatetimeParam() const;
		operator vemt::type::PhaseParam() const;
		operator vemt::type::StringParam() const;
		operator vemt::type::WstringParam() const;
	private:
		std::unique_ptr<int64_t> int_value_;
		std::unique_ptr<double> real_value_;
		std::unique_ptr<std::string> text_value_;
		std::unique_ptr<std::vector<unsigned char>> blob_value_;
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
	void bindPhase		(const int index, const type::PhaseParam & value);
	void bindPhase		(const std::string & target, const type::PhaseParam & value);

	bool step();

	std::unordered_map<std::string, GeneralValue> fetch();

	void reset();
	void clear();

private:
	sqlite3_stmt * stmt_;
	int latest_code_;
};
} // namespace db
} // namespace vemt
#endif
