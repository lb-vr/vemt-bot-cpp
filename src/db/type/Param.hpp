#ifndef VEMT_DB_TYPE_PARAM_HPP
#define VEMT_DB_TYPE_PARAM_HPP
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
namespace vemt{
namespace db{
namespace type{

template <class T> class Param{
public:
    Param(){}
    Param(T value){
        this->set(value);
    }
    virtual ~Param(){}
    const T get() const{
        return this->value;
    }
    void set(T value){
        this->value = value;
        this->is_dirty = true;
    }
    void clear(){
        this->is_dirty = false;
    }

private:
    T value;
    bool is_dirty;

};

template class Param<int>;
class IntParam : public Param<int>{
public:
    IntParam(){}
    IntParam(int value) : Param(value){}
};

template class Param<bool>;
class BoolParam : public Param<bool>{
public:
    BoolParam(){}
    BoolParam(bool value) : Param(value){}
    const int getAsInt() const{
        return (this->get()) ? 1 : 0;
    }
    void setAsInt(int value){
        this->set( value ? true : false );
    }
};

//using DoubleParam = Param<double>;

template class Param<std::chrono::system_clock::time_point>;
class DatetimeParam : public Param<std::chrono::system_clock::time_point>{
public:
    DatetimeParam(){}
    DatetimeParam(std::chrono::system_clock::time_point value) : Param(value){}
    DatetimeParam(std::string v){
        this->setAsString(v);
    }
    DatetimeParam(std::string v, std::string format){
        this->setAsString(v, format);
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
};

template class Param<std::string>;
class StringParam : public Param<std::string>{
public:
    StringParam(){}
    StringParam(std::string value) : Param(value){}
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
};

}
}
}

#endif