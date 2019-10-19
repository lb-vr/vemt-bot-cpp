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
		const vemt::type::IntParam & id,
		const vemt::type::WstringParam & text,
		const vemt::type::WstringParam & detail_text,
		const vemt::type::AnswerTypeParam & type,
		const vemt::type::WstringParam & regex_rule,
		const std::vector<vemt::type::WstringParam> & choise,
		const vemt::type::IntParam & length,
		const vemt::type::BoolParam & is_required,
		const vemt::type::IntParam & required_when_phase,
		const vemt::type::DatetimeParam & required_when_datetime,
		const vemt::type::BoolParam & multiline,
		const vemt::type::DatetimeParam & created_at
	) noexcept;

	/// @brief Jsonパース用のコンストラクタ
	///
	/// idについてはダミーを入れる？
	QuestionItemModel(
		const vemt::type::WstringParam & text,
		const vemt::type::WstringParam & detail_text,
		const vemt::type::AnswerTypeParam & type,
		const vemt::type::WstringParam & regex_rule,
		const std::vector<vemt::type::WstringParam> & choise,
		const vemt::type::IntParam & length,
		const vemt::type::BoolParam & is_required,
		const vemt::type::IntParam & required_when_phase,
		const vemt::type::DatetimeParam & required_when_datetime,
		const vemt::type::BoolParam & multiline
	) noexcept;

	/// @brief コピーコンストラクタ
	QuestionItemModel(const QuestionItemModel & copy) noexcept;

	/// @brief デストラクタ
	~QuestionItemModel();

	// -- Get Accessor --
	const vemt::type::IntParam & getId() const;
	const vemt::type::WstringParam & getText() const;
	const vemt::type::WstringParam & getDetailText() const;
	const vemt::type::AnswerTypeParam & getType() const;
	const vemt::type::WstringParam & getRegexRule() const;
	const std::vector<vemt::type::WstringParam> & getChoise() const;
	const vemt::type::IntParam & getLength() const;
	const vemt::type::BoolParam & getIsRequired() const;
	const vemt::type::IntParam & getRequiredWhenPhase() const;
	const vemt::type::DatetimeParam & getRequireWhenDatetime() const;
	const vemt::type::BoolParam & getMultiline() const;
	const vemt::type::DatetimeParam & getCreatedAt() const;

	// -- Set Accessor --
	void setText(const vemt::type::WstringParam & value);
	void setDetailText(const vemt::type::WstringParam & value);
	void setType(const vemt::type::AnswerTypeParam & value);
	void setRegexRule(const vemt::type::WstringParam & value);
	void setChoise(const std::vector<vemt::type::WstringParam> & value);
	void setLength(const vemt::type::IntParam & value);
	void setIsRequired(const vemt::type::BoolParam & value);
	void setRequiredWhenPhase(const vemt::type::IntParam & value);
	void setRequireWhenDatetime(const vemt::type::DatetimeParam & value);
	void setMultiline(const vemt::type::BoolParam & value);

	// -- utility --
	std::string toString() const;

private:
	const vemt::type::IntParam id_;
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