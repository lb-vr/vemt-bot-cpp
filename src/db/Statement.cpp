#include "Statement.hpp"
#define SQLITE_ENABLE_COLUMN_METADATA
#include "sqlite3.h"
#include <mutex>

vemt::db::BindException::BindException(const int error_code, const int index, const type::ParamBase & value)
	: DatabaseException(error_code, std::string("Failed to bind value. [" + std::to_string(index) + "] = " + value.toString()).c_str()) {}

vemt::db::BindException::BindException(const int error_code, const int index)
	: DatabaseException(error_code, std::string("Failed to bind value. [" + std::to_string(index) + "] = null").c_str()) {} 

vemt::db::Statement::Statement(sqlite3 * pdb, const std::string & sql) : stmt_(nullptr), latest_code_(SQLITE_OK){
	this->latest_code_ = ::sqlite3_prepare_v2(pdb, sql.c_str(), sql.length(), &this->stmt_, nullptr);
	if (this->latest_code_ != SQLITE_OK) throw DatabaseException(this->latest_code_, "Failed to prepare statement.");
}

vemt::db::Statement::~Statement() noexcept {
	::sqlite3_finalize(this->stmt_);
}

void vemt::db::Statement::bindInt(const int index, const type::IntParam & value) {
	if (value.isSet()) {
		this->latest_code_ = ::sqlite3_bind_int64(this->stmt_, index, value.get());
		if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index, value);
	}
}

void vemt::db::Statement::bindInt(const std::string & target, const type::IntParam & value) {
	this->bindInt(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()), value.get());
}

void vemt::db::Statement::bindBool(const int index, const type::BoolParam & value) {
	if (value.isSet()) {
		this->latest_code_ = ::sqlite3_bind_int(this->stmt_, index, value.getAsInt());
		if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index, value);
	}
}

void vemt::db::Statement::bindBool(const std::string & target, const type::BoolParam & value) {
	this->bindBool(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()), value);
}

void vemt::db::Statement::bindNull(const int index) {
	this->latest_code_ = ::sqlite3_bind_null(this->stmt_, index);
	if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index);
}

void vemt::db::Statement::bindNull(const std::string & target) {
	this->bindNull(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()));
}

void vemt::db::Statement::bindString(const int index, const type::StringParam & value) {
	if (value.isSet()) {
		auto str = value.get();
		this->latest_code_ = ::sqlite3_bind_text(this->stmt_, index, str.c_str(), str.length(), nullptr);
		if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index, value);
	}
}

void vemt::db::Statement::bindString(const std::string & target, const type::StringParam & value) {
	this->bindString(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()), value);
}

void vemt::db::Statement::bindWstring(const int index, const type::WstringParam & value) {
	if (value.isSet()) {
		auto str = value.toString();
		this->latest_code_ = ::sqlite3_bind_text(this->stmt_, index, str.c_str(), str.length(), nullptr);
		if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index, value);
	}
}

void vemt::db::Statement::bindWstring(const std::string & target, const type::WstringParam & value) {
	this->bindWstring(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()), value);
}

void vemt::db::Statement::bindDatetime(const int index, const type::DatetimeParam & value) {
	if (value.isSet()) {
		auto str = value.toString();
		this->latest_code_ = ::sqlite3_bind_text(this->stmt_, index, str.c_str(), str.length(), nullptr);
		if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index, value);
	}
}

void vemt::db::Statement::bindDatetime(const std::string & target, const type::DatetimeParam & value) {
	this->bindDatetime(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()), value);
}

void vemt::db::Statement::bindPhase(const int index, const type::PhaseParam & value) {
	if (value.isSet()) {
		auto int_val = value.getAsInt();
		this->latest_code_ = ::sqlite3_bind_int(this->stmt_, index, int_val);
		if (this->latest_code_ != SQLITE_OK) throw BindException(this->latest_code_, index, value);
	}
}

void vemt::db::Statement::bindPhase(const std::string & target, const type::PhaseParam & value) {
	this->bindPhase(::sqlite3_bind_parameter_index(this->stmt_, target.c_str()), value);
}

namespace { std::mutex mtx; }
bool vemt::db::Statement::step() {
	std::lock_guard<std::mutex> lock(mtx);
	this->latest_code_ = ::sqlite3_step(this->stmt_);
	if (this->latest_code_ != SQLITE_DONE && this->latest_code_ != SQLITE_ROW)
		throw DatabaseException(this->latest_code_, "Failed to step().");
	return (this->latest_code_ == SQLITE_ROW);
}

std::unordered_map<std::string, vemt::db::Statement::GeneralValue> vemt::db::Statement::fetch() {
	if (this->latest_code_ != SQLITE_ROW) {
		throw FetchException(-1, "Failed to fetch.");
	}

	std::unordered_map<std::string, vemt::db::Statement::GeneralValue> ret;
	const int column_count = ::sqlite3_column_count(this->stmt_);
	for (int i = 0; i < column_count; i++) {
		const char * column_name_carray = ::sqlite3_column_name(this->stmt_, i);
		assert(column_name_carray != nullptr);
		std::string column_name(column_name_carray);
		std::string key = column_name;
		assert(ret.count(key) == 0);	// Duplicated key.

		auto type = ::sqlite3_column_type(this->stmt_, i);
		switch (type) {
		case SQLITE_INTEGER:
			ret.at(key).setAsInt(::sqlite3_column_int64(this->stmt_, i));
			break;
		case SQLITE_FLOAT:
			ret.at(key).setAsReal(::sqlite3_column_double(this->stmt_, i));
			break;
		case SQLITE3_TEXT:
			ret.at(key).setAsString(::sqlite3_column_text(this->stmt_, i), ::sqlite3_column_bytes(this->stmt_, i));
			break;
		case SQLITE_BLOB:
			ret.at(key).setAsBlob(::sqlite3_column_blob(this->stmt_, i), ::sqlite3_column_bytes(this->stmt_, i));
			break;
		}
	}
	return ret;
}

void vemt::db::Statement::reset() {
	this->latest_code_ = ::sqlite3_reset(this->stmt_);
	if (this->latest_code_ != SQLITE_OK) throw DatabaseException(this->latest_code_, "Failed to reset statement.");
}

void vemt::db::Statement::clear() {
	this->latest_code_ = ::sqlite3_clear_bindings(this->stmt_);
	if (this->latest_code_ != SQLITE_OK) throw DatabaseException(this->latest_code_, "Failed to clear bindings.");
}

vemt::db::Statement::GeneralValue::GeneralValue() noexcept {}

vemt::db::Statement::GeneralValue::GeneralValue(const GeneralValue & copy) noexcept
	: int_value_(copy.int_value_ ? std::make_unique<int64_t>(*copy.int_value_) : nullptr),
	real_value_(copy.real_value_ ? std::make_unique<double>(*copy.real_value_) : nullptr),
	text_value_(copy.text_value_ ? std::make_unique<std::string>(*copy.text_value_) : nullptr),
	blob_value_(copy.blob_value_ ? std::make_unique<std::vector<unsigned char>>(*copy.blob_value_) : nullptr)
{}

vemt::db::Statement::GeneralValue::GeneralValue(GeneralValue && move) noexcept
	: int_value_(std::move(move.int_value_)),
	real_value_(std::move(move.real_value_)),
	text_value_(std::move(move.text_value_)),
	blob_value_(std::move(move.blob_value_))
{}

vemt::db::Statement::GeneralValue & vemt::db::Statement::GeneralValue::operator=(const GeneralValue & v) {
	this->int_value_ = (v.int_value_ ? std::make_unique<int64_t>(*v.int_value_) : nullptr);
	this->real_value_ = (v.real_value_ ? std::make_unique<double>(*v.real_value_) : nullptr);
	this->text_value_ = (v.text_value_ ? std::make_unique<std::string>(*v.text_value_) : nullptr);
	this->blob_value_ = (v.blob_value_ ? std::make_unique<std::vector<unsigned char>>(*v.blob_value_) : nullptr);
	return *this;
}

vemt::db::Statement::GeneralValue::~GeneralValue() {}

void vemt::db::Statement::GeneralValue::setAsInt(const int64_t value) {
	this->int_value_ = std::make_unique<int64_t>(value);
}

void vemt::db::Statement::GeneralValue::setAsReal(const double value) {
	this->real_value_ = std::make_unique<double>(value);
}

void vemt::db::Statement::GeneralValue::setAsString(const unsigned char * const text, const int bytes) {
	this->text_value_ = std::make_unique<std::string>();
	for (int i = 0; i < bytes; i++) this->text_value_->push_back(text[i]);
}

void vemt::db::Statement::GeneralValue::setAsBlob(const void * const blob, const int bytes) {
	this->blob_value_ = std::make_unique<std::vector<unsigned char>>(bytes);
	for (int i = 0; i < bytes; i++) this->blob_value_->at(static_cast<const char*>(blob)[i]);
}

vemt::type::IntParam vemt::db::Statement::GeneralValue::getAsInt() const {
	type::IntParam ret;
	if (this->int_value_) ret.set(*this->int_value_);
	return ret;
}

vemt::type::BoolParam vemt::db::Statement::GeneralValue::getAsBool() const {
	type::BoolParam ret;
	if (this->int_value_) ret.setAsInt(*this->int_value_);
	return ret;
}

vemt::type::DatetimeParam vemt::db::Statement::GeneralValue::getAsDatetime() const {
	type::DatetimeParam ret;
	if (this->text_value_) ret.setAsString(*this->text_value_);
	return ret;
}

vemt::type::PhaseParam vemt::db::Statement::GeneralValue::getAsPhase() const {
	type::PhaseParam ret;
	if (this->int_value_) ret.setAsInt(*this->int_value_);
	return ret;
}

vemt::type::StringParam vemt::db::Statement::GeneralValue::getAsString() const {
	type::StringParam ret;
	if (this->text_value_) ret.set(*this->text_value_);
	return ret;
}

vemt::type::WstringParam vemt::db::Statement::GeneralValue::getAsWstring() const {
	type::WstringParam ret;
	if (this->text_value_) ret.setAsString(*this->text_value_);
	return ret;
}

std::vector<unsigned char> vemt::db::Statement::GeneralValue::getAsBlob() const{
	if (this->blob_value_) return *this->blob_value_;
	return std::vector<unsigned char>();
}


vemt::db::Statement::GeneralValue::operator vemt::type::IntParam() const { return this->getAsInt(); }
vemt::db::Statement::GeneralValue::operator vemt::type::BoolParam() const { return this->getAsBool(); }
vemt::db::Statement::GeneralValue::operator vemt::type::DatetimeParam() const { return this->getAsDatetime(); }
vemt::db::Statement::GeneralValue::operator vemt::type::PhaseParam() const { return this->getAsPhase(); }
vemt::db::Statement::GeneralValue::operator vemt::type::StringParam() const { return this->getAsString(); }
vemt::db::Statement::GeneralValue::operator vemt::type::WstringParam() const { return this->getAsWstring(); }
