﻿#ifndef VEMT_QUESTIONITEM_HPP
#define VEMT_QUESTIONITEM_HPP
#include "type/Phase.hpp"

#include "db/QuestionItemModel.hpp"

#include <vector>
#include <string>
#include <regex>

namespace json11 {
class Json;	// 前方参照
}

namespace vemt {
namespace bot {

class JsonParseError : public std::invalid_argument {
public:
	explicit JsonParseError(const std::wstring & str) noexcept;
};

class QuestionItem : public db::QuestionItemModel {
public:


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
		const type::AnswerType type,
		const std::wstring & regex_rule,
		const std::vector<std::wstring> choise,
		const int & length,
		const bool is_required,
		const type::PhaseParam required_when_phase,
		const type::DatetimeParam & required_when_datetime,
		const bool & multiline) noexcept;

	explicit QuestionItem(const db::QuestionItemModel & model) noexcept;

	/// @brief デストラクタ
	~QuestionItem();

	/// @brief 答えの文字列に対してバリデーティングをする
	ValidationResult validate(const std::wstring & answer) const;

	// -- utility --
	std::string toString() const;

	std::wstring createFullMessage() const;

	// -- parser ---
	static QuestionItem loadFromJson(const json11::Json & json);


private:
	static std::vector<type::WstringParam> _toWstringVector(const std::vector<std::wstring> & wvec);
};



}
}

#endif