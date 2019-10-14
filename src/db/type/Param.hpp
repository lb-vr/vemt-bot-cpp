#ifndef VEMT_DB_TYPE_PARAM_HPP
#define VEMT_DB_TYPE_PARAM_HPP
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <memory>
#include <cstdlib>

namespace vemt{
namespace db{
namespace type{

template <class T> class Param{
public:
    Param() : value_(nullptr){}
    Param(const T value){
        this->set(value);
    }
    Param(const Param & src) : Param(src.get()){}
    Param & operator=(const Param & param) noexcept{
        this->set(param.get());
        return *this;
    }

    virtual ~Param(){}
    const T get() const{
        return *this->value_;
    }
    void set(T value){
        if(! this->isAcceptable(value)){
            std::cerr << "UNACCEPTABLE VALUE" << std::endl;
            std::exit(-1);
        }
        if(!this->value_){
            this->value_ = std::make_unique<T>();
        }
        *this->value_ = value;
    }
    bool isAcceptable(T value) const{return true;} // const = 0 ;
    const std::string toString() const {
        return std::string("Param dummy");
    };

private:
    std::unique_ptr<T> value_;
};

template class Param<int>;
class IntParam : public Param<int>{
    using Param<int>::Param;
    const std::string toString(){
        std::stringstream ss;
        ss << this->get();
        return ss.str();
    }
};

template class Param<bool>;
class BoolParam : public Param<bool>{
public:
    using Param<bool>::Param;
    const int getAsInt() const{
        return (this->get()) ? 1 : 0;
    }
    void setAsInt(int value){
        this->set( value ? true : false );
    }
    const std::string toString(){
        std::stringstream ss;
        ss << this->get();
        return ss.str();
    }
};

//using DoubleParam = Param<double>;

template class Param<std::chrono::system_clock::time_point>;
class DatetimeParam : public Param<std::chrono::system_clock::time_point>{
public:
    using Param<std::chrono::system_clock::time_point>::Param;
    DatetimeParam(std::string v){
        this->setAsString(v);
    }
    const int getAsInt() const{
        return std::chrono::duration_cast<std::chrono::seconds>(
            this->get().time_since_epoch()
        ).count();
    }
    void setAsInt(int v){
        this->set(
            std::chrono::system_clock::from_time_t(v)
        );
    }

    const std::string getAsString() const{
        std::stringstream ss;
        std::time_t _value = std::chrono::system_clock::to_time_t(this->get());
        const tm*  __value = std::localtime(&_value);
        ss << std::put_time(__value, "%F %T");
        return ss.str();
    }
    void setAsString(std::string v, std::string format="%Y-%m-%d %H:%M:%S"){
        std::stringstream ss(v);
        std::tm tm = {};
        ss >> std::get_time(&tm, format.c_str());
        this->set(std::chrono::system_clock::from_time_t(std::mktime(&tm)));
    }

    const std::string toString(){
        return this->getAsString();
    }
};

template class Param<std::string>;
class StringParam : public Param<std::string>{
public:
    using Param<std::string>::Param;
    StringParam(const unsigned char *c_str, size_t len) : Param(){
        this->setAsCStr(c_str, len);
    }
    void setAsCStr(const unsigned char *c_str, size_t len){
        std::string ret = "";
        for (size_t l = 0; l < len; l++){
            ret.push_back(c_str[l]);
        }
        this->set(ret);
    }
    const std::string toString(){
        return this->get();
    }
};

}
}
}

#endif