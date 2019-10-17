#ifndef VEMT_TYPE_PARAM_HPP
#define VEMT_TYPE_PARAM_HPP

#include <cassert>
#include <memory>
#include <string>

namespace vemt{
namespace type{

template <typename T> class Param{
public:
	Param() noexcept : value_(nullptr) {}
	Param(const T &) = delete;
	virtual ~Param() {}
	Param<T> & operator=(const Param<T> & param) noexcept {
		if (param.isSet()) this->set(param.get());
		return *this;
	}
	Param<T> & operator=(const T & value) noexcept { this->set(value); return *this; }
	const T get() const { assert(this->isSet()); return *this->value_; }
	void set(const T & value) {
		assert(this->isAcceptable(value));
		if (!this->value_) this->value_ = std::make_unique<T>();
		*this->value_ = value;
	}
	bool isSet() const { return static_cast<bool>(this->value_); }
	operator bool() const { return this->isSet(); }
	virtual bool isAcceptable(const T & value) const = 0;
	virtual const std::string toString() const { return "Param dummy."; }
protected:
	const std::unique_ptr<T> & getValueUptr() const { return this->value_; }
private:
	std::unique_ptr<T> value_;
};




}
}

#endif