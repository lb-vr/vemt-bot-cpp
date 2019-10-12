#ifndef VEMT_QUESTIONITEMMODEL_HPP
#define VEMT_QUESTIONITEMMODEL_HPP

#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <regex>

#include "type/Param.hpp"
#include "type/BoolParam.hpp"
#include "type/StringParam.hpp"
#include "type/DatetimeParam.hpp"


namespace vemt {
namespace db {

class QuestionItemModel {
public:
	/// @brief Database用のコンストラクタ
	QuestionItemModel(
		const vemt::db::type::IntParam & id,
		const vemt::db::type::StringParam & text,
		const vemt::db::type::StringParam & detail_text,
		const vemt::db::type::IntParam type,
		const vemt::db::type::StringParam & regex_rule,
		const std::vector<vemt::db::type::StringParam> choise,
		const vemt::db::type::IntParam & length,
		const vemt::db::type::BoolParam & is_required,
		const vemt::db::type::IntParam & required_when_phase,
		const vemt::db::type::DatetimeParam & required_when_datetime,
		const vemt::db::type::BoolParam & multiline,
		const vemt::db::type::DatetimeParam & created_at
	) noexcept;

	/// @brief Jsonパース用のコンストラクタ
	///
	/// idについてはダミーを入れる？
	QuestionItemModel(
		const vemt::db::type::StringParam & text,
		const vemt::db::type::StringParam & detail_text,
		const vemt::db::type::IntParam type,
		const vemt::db::type::StringParam & regex_rule,
		const std::vector<vemt::db::type::StringParam> & choise,
		const vemt::db::type::IntParam length,
		const vemt::db::type::BoolParam is_required,
		const vemt::db::type::IntParam required_when_phase,
		const vemt::db::type::DatetimeParam required_when_datetime,
		const vemt::db::type::BoolParam multiline
	) noexcept;

	/// @brief コピーコンストラクタ
	QuestionItemModel(const QuestionItemModel & copy) noexcept;

	/// @brief デストラクタ
	~QuestionItemModel();

	// -- Get Accessor --
	const int getId() const;
	const std::string & getText() const;
	const std::string & getDetailText() const;
	const int getType() const;
	const std::string getRegex() const;
	const std::string getRegexRule() const;
	const std::vector<std::string> & getChoise() const;
	const int getLength() const;
	const bool getIsRequired() const;
	const int getRequiredWhenPhase() const;
	const std::chrono::system_clock::time_point getRequireWhenDatetime() const;
	const bool getMultiline() const;
	const std::chrono::system_clock::time_point getCreatedAt() const;

	// -- Set Accessor --
	void setText(const std::string & text);
	void setDetailText(const std::string & detail_text);
	void setType(const int type);
	void setRegexRule(const std::string & regex_rule);
	void setChoise(const std::vector<std::string> choise);
	void setLength(const int length);
	void setIsRequired(const bool required);
	void setRequiredWhenPhase(const int phase);
	void setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint);
	void setMultiline(const bool multiline);

	// -- utility --
	std::string toString() const;

private:
	const vemt::db::type::IntParam id_;
	vemt::db::type::StringParam text_;
	vemt::db::type::StringParam detail_text_;
	vemt::db::type::IntParam type_;
	vemt::db::type::StringParam regex_rule_;
	std::vector<vemt::db::type::StringParam> choise_;
	vemt::db::type::IntParam length_;
	vemt::db::type::BoolParam is_required_;
	vemt::db::type::IntParam required_when_phase_;
	vemt::db::type::DatetimeParam required_when_datetime_;
	vemt::db::type::BoolParam multiline_;
	vemt::db::type::DatetimeParam created_at_;
};

}
}

#endif