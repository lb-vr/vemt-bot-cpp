#ifndef VEMT_DB_REGISTRYTABLE_INCLUDED
#define VEMT_DB_REGISTRYTABLE_INCLUDED
#include <sstream>
#include <string>
#include "type/Type.hpp"
#include "sqlite3.h"
#include "BaseTable.hpp"

namespace vemt
{
namespace db
{

class RegistryTable : public BaseTable
{
public:
    static std::string getIntTableName();
    static std::string getBoolTableName();
    static std::string getStringTableName();
    static std::string getWstringTableName();
    static std::string getDatetimeTableName();

    RegistryTable(const std::string & dbPath) noexcept;

    const vemt::type::IntParam      & getInt(const std::string & key);
    const vemt::type::BoolParam     & getBool(const std::string & key);
    const vemt::type::StringParam   & getString(const std::string & key);
    const vemt::type::WstringParam  & getWstring(const std::string & key);
    const vemt::type::DatetimeParam & getDatetime(const std::string & key);

    void setInt     (const std::string & key, vemt::type::IntParam      & value);
    void setBool    (const std::string & key, vemt::type::BoolParam     & value);
    void setString  (const std::string & key, vemt::type::StringParam   & value);
    void setWstring (const std::string & key, vemt::type::WstringParam  & value);
    void setDatetime(const std::string & key, vemt::type::DatetimeParam & value);
};
} // namespace db
} // namespace vemt
#endif
