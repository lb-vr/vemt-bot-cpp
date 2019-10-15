#include "Param.hpp"
#include "Logger.hpp"
#include <cstdlib>


template<class T>
vemt::db::type::Param<T>::Param() noexcept : value_(nullptr) {}

template<class T>
vemt::db::type::Param<T>::Param(const T value) : Param() 
{ this->set(value); }

template<class T>
vemt::db::type::Param<T>::Param(const Param & src) : Param() {
	if (src.isSet()) this->set(src.get());
}

template<class T>
vemt::db::type::Param<T> & vemt::db::type::Param<T>::operator=(const Param<T> & param) noexcept {
	if (param.isSet()) this->set(param.get());
	return *this;
}

template<class T>
const T vemt::db::type::Param<T>::get() const {
	wAssertM(!this->isSet(), "NON-INITIALIZED VALUE.");
	if (!this->isAcceptable(*this->value_)) {
		wAssertM(false, "UNACCEPTABLE VALUE");
		std::exit(-1);
	}
	return *this->value_; 
}

template<class T>
void vemt::db::type::Param<T>::set(const T & value) {
	if (!this->value_) {
		this->value_ = std::make_unique<T>();
	}
	*this->value_ = value;
}

template<typename T>
bool vemt::db::type::Param<T>::isSet() const
{ return static_cast<bool>(this->value_); }

template<typename T>
vemt::db::type::Param<T>::operator bool() const
{ return this->isSet(); }

template<class T>
const std::string vemt::db::type::Param<T>::toString() const{
	return std::string("Param dummy");
}