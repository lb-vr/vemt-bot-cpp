#include "Param.hpp"
#include "util/Logger.hpp"
#include <cstdlib>


template<class T>
vemt::type::Param<T>::Param() noexcept : value_(nullptr) {
	logging::trace << "Param{" << this << "}::Default Constructor called. << " << this->getValueUptr().get() << " >>" << std::endl;
}

template<typename T>
vemt::type::Param<T>::~Param() {
	logging::trace << "Param{" << this << "}::Deleter called. << " << this->getValueUptr().get() << " >>" << std::endl;
}

template<class T>
vemt::type::Param<T> & vemt::type::Param<T>::operator=(const Param<T> & param) noexcept {
	if (param.isSet()) this->set(param.get());
	return *this;
}

template<typename T>
vemt::type::Param<T> & vemt::type::Param<T>::operator=(const T & value) noexcept {
	this->set(value);
	return *this;
}

template<class T>
const T vemt::type::Param<T>::get() const {
	wAssertM(this->isSet(), "NON-INITIALIZED VALUE.");
	logging::trace << "Param{" << this << "}::get() called. << " << this->getValueUptr().get() << " >>" << std::endl;
	return *this->value_; 
}

template<class T>
void vemt::type::Param<T>::set(const T & value) {
	if (!this->isAcceptable(value)) {
		wAssertM(false, "UNACCEPTABLE VALUE");
		std::exit(-1);
	}
	if (!this->value_) {
		this->value_ = std::make_unique<T>();
	}
	*this->value_ = value;
}

template<typename T>
bool vemt::type::Param<T>::isSet() const
{ return static_cast<bool>(this->value_); }

template<typename T>
vemt::type::Param<T>::operator bool() const
{ return this->isSet(); }

template<class T>
const std::string vemt::type::Param<T>::toString() const{
	return std::string("Param dummy");
}

template<typename T>
const std::unique_ptr<T>& vemt::type::Param<T>::getValueUptr() const
{ return this->value_; }
