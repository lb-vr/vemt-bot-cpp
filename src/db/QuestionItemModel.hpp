#ifndef VEMT_QUESTIONITEMMODEL_HPP
#define VEMT_QUESTIONITEMMODEL_HPP
#include "type/Type.hpp"
#include <vector>

namespace vemt {
namespace db {

class QuestionItemModel {
public:
	/// @brief Database用のコンストラクタ
	QuestionItemModel(
		const type::IntParam & id,
		const type::WstringParam & text,
		const type::WstringParam & detail_text,
		const type::AnswerTypeParam type,
		const type::WstringParam & regex_rule,
		const std::vector<vemt::type::WstringParam> choise,
		const type::IntParam & length,
		const type::BoolParam & is_required,
		const type::IntParam & required_when_phase,
		const type::DatetimeParam & required_when_datetime,
		const type::BoolParam & multiline,
		const type::DatetimeParam & created_at
	) noexcept;

	/// @brief Jsonパース用のコンストラクタ
	///
	/// idについてはダミーを入れる？
	QuestionItemModel(
		const type::WstringParam & text,
		const type::WstringParam & detail_text,
		const type::AnswerTypeParam type,
		const type::WstringParam & regex_rule,
		const std::vector<vemt::type::WstringParam> & choise,
		const type::IntParam length,
		const type::BoolParam is_required,
		const type::IntParam required_when_phase,
		const type::DatetimeParam required_when_datetime,
		const type::BoolParam multiline
	) noexcept;

	/// @brief コピーコンストラクタ
	QuestionItemModel(const QuestionItemModel & copy) noexcept;

	/// @brief デストラクタ
	~QuestionItemModel();

	// -- Get Accessor --
	const int getId() const;
	const std::wstring getText() const;
	const std::wstring getDetailText() const;
	const type::AnswerType getType() const;
	const std::wstring getRegexRule() const;
	const std::vector<std::wstring> getChoise() const;
	const int getLength() const;
	const bool getIsRequired() const;
	const int getRequiredWhenPhase() const;
	const time_t getRequireWhenDatetime() const;
	const bool getMultiline() const;
	const time_t getCreatedAt() const;

	// -- Set Accessor --
	void setText(const std::wstring & text);
	void setDetailText(const std::wstring & detail_text);
	void setType(const type::AnswerType type);
	void setRegexRule(const std::wstring & regex_rule);
	void setChoise(const std::vector<std::wstring> choise);
	void setLength(const int length);
	void setIsRequired(const bool required);
	void setRequiredWhenPhase(const int phase);
	void setRequiredWhenDatetime(const time_t & timepoint);
	void setMultiline(const bool multiline);

	// -- utility --
	std::string toString() const;

private:
	const type::IntParam id_;
	type::WstringParam text_;
	type::WstringParam detail_text_;
	type::AnswerTypeParam type_;
	type::WstringParam regex_rule_;
	std::vector<type::WstringParam> choise_;
	type::IntParam length_;
	type::BoolParam is_required_;
	type::IntParam required_when_phase_;
	type::DatetimeParam required_when_datetime_;
	type::BoolParam multiline_;
	type::DatetimeParam created_at_;
};

}
}

#endif