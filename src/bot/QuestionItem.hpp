#ifndef VEMT_QUESTIONITEM_HPP
#define VEMT_QUESTIONITEM_HPP

#include "Phase.hpp"
#include "db/QuestionItemModel.hpp"
#include "bot/AnswerType.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <regex>

namespace json11 {
class Json;	// 前方参照
}

namespace vemt {
namespace bot {

class QuestionItem : public db::QuestionItemModel {
public:

	class JsonParseError : public std::invalid_argument {
	public:
		explicit JsonParseError(const std::wstring & str) noexcept;
	};

	enum ValidationResult {
		kOk = 0,				///< 正常
		kAnswerEmpty,			///< 回答が空だった場合
		kUnexceptedType,		///< 回答のタイプがミスマッチだった
		kUnmatchRegex,			///< 正規表現にマッチしなかった
		kOutOfChoise,			///< 選択項目以外だった
		kOverMaxLength,			///< 回答文字が規定以上
		kUneditablePhase,		///< 再編集不可能なフェーズ
		kUneditableDatetime,	///< 再編集不可能な時間
	};

	explicit QuestionItem(
		const std::wstring & text,
		const std::wstring & detail_text,
		const AnswerType type,
		const std::wstring & regex_rule,
		const std::vector<std::wstring> choise,
		const int & length,
		const bool is_required,
		const int required_when_phase,
		const db::type::DatetimeParam & required_when_datetime,
		const bool & multiline) noexcept;

	explicit QuestionItem(const db::QuestionItemModel & model) noexcept;

	/// @brief デストラクタ
	~QuestionItem();

	/// @brief 答えの文字列に対してバリデーティングをする
	ValidationResult validate(const std::wstring & answer) const;

	// -- utility --
	std::string toString() const;

	// -- parser ---
	static QuestionItem createFromJson(const json11::Json & json, std::string & error_msg);

private:
};



}
}

#endif