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
	Param(const T value);
	Param(const Param & src);
    virtual ~Param(){}

	Param & operator=(const Param & param) noexcept;

	const T get() const;
	void set(const T & value);
	bool isSet() const;
	operator bool() const;
	virtual bool isAcceptable(const T & value) const = 0;
	virtual const std::string toString() const;
private:
    std::unique_ptr<T> value_;
};

template class Param<int>;
class IntParam : public Param<int>{
    using Param<int>::Param;
	virtual bool isAcceptable(const int & value) const override;
	virtual const std::string toString() const override;
};

template class Param<bool>;
class BoolParam : public Param<bool>{
public:
    using Param<bool>::Param;
	const int getAsInt() const;
	void setAsInt(int value);
	virtual bool isAcceptable(const bool & value) const override;
	virtual const std::string toString() const override;
};

/*
template class Param<std::chrono::system_clock::time_point>;
class DatetimeParam : public Param<std::chrono::system_clock::time_point>{
public:
    using Param<std::chrono::system_clock::time_point>::Param;
	//DatetimeParam(void);
	//DatetimeParam(const std::chrono::system_clock::time_point & t);
	//DatetimeParam(const DatetimeParam & p);

	//DatetimeParam(const std::string & v);
	const int getAsInt() const;
	void setAsInt(const int v);

	const std::string getAsString() const;
	void setAsString(const std::string & v, const std::string & format = "%Y-%m-%d %H:%M:%S");

	virtual bool isAcceptable(const std::chrono::system_clock::time_point & value) const override;
	virtual const std::string toString() const override;
};
*/
template class Param<time_t>;
class DatetimeParam : public Param<time_t> {
public:
	using Param<time_t>::Param;
	//DatetimeParam(void);
	//DatetimeParam(const std::chrono::system_clock::time_point & t);
	//DatetimeParam(const DatetimeParam & p);

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
    using Param<std::wstring>::Param;
	void setAsCStr(const unsigned char *c_str, size_t len);
	virtual bool isAcceptable(const std::wstring & value) const override;
	virtual const std::string toString() const override;
};

template class Param<std::string>;
class StringParam : public Param<std::string> {
public:
	using Param<std::string>::Param;
	//StringParam(const unsigned char *c_str, size_t len);
	void setAsCStr(const unsigned char *c_str, size_t len);
	virtual bool isAcceptable(const std::string & value) const override;
	virtual const std::string toString() const override;
};

template class Param<AnswerType>;
class AnswerTypeParam : public Param<AnswerType> {
public:
	class ParseError : public std::exception {};
	using Param<AnswerType>::Param;
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