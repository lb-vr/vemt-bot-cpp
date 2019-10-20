#include "Statement.hpp"
#include "sqlite3.h"

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

void vemt::db::Statement::step() {
	this->latest_code_ = ::sqlite3_step(this->stmt_);
	if (this->latest_code_ != SQLITE_DONE && this->latest_code_ != SQLITE_ROW)
		throw DatabaseException(this->latest_code_, "Failed to step().");
}

std::unordered_map<std::string, vemt::db::Statement::GeneralValue> vemt::db::Statement::fetch() {
	if (this->latest_code_ != SQLITE_ROW) {
		throw FetchException(-1, "Failed to fetch.");
	}

	std::unordered_map<std::string, vemt::db::Statement::GeneralValue> ret;
	const int column_count = ::sqlite3_column_count(this->stmt_);
	for (int i = 0; i < column_count; i++) {
		std::string column_name = ::sqlite3_column_name(this->stmt_, i);
		auto val = ::sqlite3_column_value(this->stmt_, i);
		ret.insert_or_assign(column_name, GeneralValue(val));
	}
	return ret;
}

void vemt::db::Statement::reset() {
	this->latest_code_ = ::sqlite3_reset(this->stmt_);
	if (this->latest_code_ != SQLITE_OK) throw DatabaseException(this->latest_code_, "Failed to reset statement.");
}

vemt::db::Statement::GeneralValue::GeneralValue(const sqlite3_value * value)
	: value_(std::make_unique<::sqlite3_value>(*value)) {}

vemt::db::Statement::GeneralValue::GeneralValue(GeneralValue && move) noexcept
	: value_(std::move(move.value_)) {}

vemt::type::IntParam vemt::db::Statement::GeneralValue::getAsInt() const {
	return type::IntParam(::sqlite3_value_int64(this->value_.get()));
}

vemt::type::BoolParam vemt::db::Statement::GeneralValue::getAsBool() const {
	type::BoolParam ret;
	ret.setAsInt(::sqlite3_value_int(this->value_.get()));
	return ret;
}

vemt::type::DatetimeParam vemt::db::Statement::GeneralValue::getAsDatetime() const {
	type::DatetimeParam dt_prm;
	type::StringParam str_prm;
	str_prm.setAsCStr(::sqlite3_value_text(this->value_.get()), ::sqlite3_value_bytes(this->value_.get()));
	dt_prm.setAsString(str_prm.toString());
	return dt_prm;
}

vemt::type::PhaseParam vemt::db::Statement::GeneralValue::getAsPhase() const {
	type::PhaseParam ret;
	ret.setAsInt(::sqlite3_value_int(this->value_.get()));
	return ret;
}

vemt::type::StringParam vemt::db::Statement::GeneralValue::getAsString() const {
	type::StringParam ret;
	ret.setAsCStr(::sqlite3_value_text(this->value_.get()), ::sqlite3_value_bytes(this->value_.get()));
	return ret;
}

vemt::type::WstringParam vemt::db::Statement::GeneralValue::getAsWstring() const {
	type::WstringParam ret;
	ret.setAsCStr(::sqlite3_value_text(this->value_.get()), ::sqlite3_value_bytes(this->value_.get()));
	return ret;
}

vemt::db::Statement::GeneralValue::operator type::IntParam() const { return this->getAsInt(); }
vemt::db::Statement::GeneralValue::operator type::BoolParam() const { return this->getAsBool(); }
vemt::db::Statement::GeneralValue::operator type::DatetimeParam() const { return this->getAsDatetime(); }
vemt::db::Statement::GeneralValue::operator type::PhaseParam() const { return this->getAsPhase(); }
vemt::db::Statement::GeneralValue::operator type::StringParam() const { return this->getAsString(); }
vemt::db::Statement::GeneralValue::operator type::WstringParam() const { return this->getAsWstring(); }
