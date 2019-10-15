#ifndef VEMT_DB_TYPE_PARAM_HPP
#define VEMT_DB_TYPE_PARAM_HPP

#include "AnswerType.hpp"
#include <string>
#include <chrono>
#include <memory>

namespace vemt{
namespace db{
namespace type{

template <typename T> class Param{
public:
	Param() noexcept;
	Param(const T &) = delete;
	virtual ~Param();

	Param & operator=(const Param & param) noexcept;
	Param & operator=(const T & value) noexcept;

	const T get() const;
	void set(const T & value);
	bool isSet() const;
	operator bool() const;
	virtual bool isAcceptable(const T & value) const = 0;
	virtual const std::string toString() const;
protected:
	const std::unique_ptr<T> & getValueUptr() const;
private:
	std::unique_ptr<T> value_;
};

template class Param<int>;
class IntParam : public Param<int>{
public:
	IntParam() noexcept;
	IntParam(const int value);
	IntParam(const IntParam & int_param);
	virtual bool isAcceptable(const int & value) const override;
	virtual const std::string toString() const override;
};

template class Param<bool>;
class BoolParam : public Param<bool>{
public:
	BoolParam() noexcept;
	BoolParam(const bool value);
	BoolParam(const BoolParam & bool_param);
	const int getAsInt() const;
	void setAsInt(int value);
	virtual bool isAcceptable(const bool & value) const override;
	virtual const std::string toString() const override;
};

template class Param<time_t>;
class DatetimeParam : public Param<time_t> {
public:
	DatetimeParam() noexcept;
	DatetimeParam(const time_t t);
	DatetimeParam(const DatetimeParam & p);
	//DatetimeParam(const std::string & v);
	
	const int getAsInt() const;
	void setAsInt(const int v);

	const std::string getAsString() const;
	void setAsString(const std::string & v, const std::string & format = "%Y-%m-%d %H:%M:%S");

	virtual bool isAcceptable(const time_t & value) const override;
	virtual const std::string toString() const override;
};

template class Param<std::wstring>;
class WstringParam : public Param<std::wstring>{
public:
	WstringParam() noexcept;
	WstringParam(const std::wstring & value);
	WstringParam(const WstringParam & wstring_param);
	void setAsCStr(const unsigned char *c_str, size_t len);
	virtual bool isAcceptable(const std::wstring & value) const override;
	virtual const std::string toString() const override;
};

template class Param<std::string>;
class StringParam : public Param<std::string> {
public:
	StringParam() noexcept;
	StringParam(const std::string & value);
	StringParam(const StringParam & string_param);
	//StringParam(const unsigned char *c_str, size_t len);
	void setAsCStr(const unsigned char *c_str, size_t len);
	virtual bool isAcceptable(const std::string & value) const override;
	virtual const std::string toString() const override;
};

template class Param<AnswerType>;
class AnswerTypeParam : public Param<AnswerType> {
public:
	class ParseError : public std::exception {};
	AnswerTypeParam() noexcept;
	AnswerTypeParam(const AnswerType value);
	AnswerTypeParam(const AnswerTypeParam & answer_type_param);
	const int getAsInt() const;
	void setAsInt(const int v);
	virtual bool isAcceptable(const AnswerType & value) const override;
	virtual const std::string toString() const override;
	static AnswerTypeParam parseFromString(const std::string & str);
};

}
}
}

#endif