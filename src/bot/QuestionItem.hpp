#ifndef VEMT_QUESTIONITEM_HPP
#define VEMT_QUESTIONITEM_HPP

#include "Phase.hpp"

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

class QuestionItem {
public:

	enum Type {
		kUnknown,
		kString,
		kNumber,
		kPicture,
		kJsonFile,
		kJson,
		kRegex
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

	/// @brief Database用のコンストラクタ
	QuestionItem(
		const unsigned long int id,
		const std::wstring & text,
		const std::wstring & detail_text,
		const Type type,
		const std::wstring & regex_rule,
		const std::vector<std::wstring> & choise,
		const unsigned int length,
		const bool is_required,
		const Phase required_when_phase,
		const std::chrono::system_clock::time_point required_when_datetime,
		const bool multiline
	) noexcept;

	/// @brief Jsonパース用のコンストラクタ
	///
	/// idについてはダミーを入れる？
	QuestionItem(
		const std::wstring & text,
		const std::wstring & detail_text,
		const Type type,
		const std::wstring & regex_rule,
		const std::vector<std::wstring> & choise,
		const unsigned int length,
		const bool is_required,
		const Phase required_when_phase,
		const std::chrono::system_clock::time_point required_when_datetime,
		const bool multiline
	) noexcept;

	/// @brief コピーコンストラクタ
	QuestionItem(const QuestionItem & copy) noexcept;

	/// @brief デストラクタ
	~QuestionItem();

	/// @brief 答えの文字列に対してバリデーティングをする
	ValidationResult validate(const std::wstring & answer) const;

	// -- Get Accessor --
	const unsigned long int getId() const;
	const std::wstring & getText() const;
	const std::wstring & getDetailText() const;
	const Type getType() const;
	const std::wregex getRegex() const;
	const std::wstring getRegexRule() const;
	const std::vector<std::wstring> & getChoise() const;
	const unsigned int getLength() const;
	const bool getIsRequired() const;
	const Phase getRequiredWhenPhase() const;
	const std::chrono::system_clock::time_point getRequireWhenDatetime() const;
	const bool getMultiline() const;

	// -- Set Accessor --
	void setText(const std::wstring & text);
	void setDetailText(const std::wstring & detail_text);
	void setType(const Type type);
	void setRegexRule(const std::wstring & regex_rule);
	void setChoise(const std::vector<std::wstring> & choise);
	void setLength(const unsigned int length);
	void setIsRequired(const bool required);
	void setRequiredWhenPhase(const Phase phase);
	void setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint);
	void setMultiline(const bool multiline);

	// -- utility --
	std::string toString() const;

	// -- parser ---
	static QuestionItem createFromJson(const json11::Json & json, std::string & error_msg);

	// -- caster --
	static std::string type2str(const Type type);
	static Type str2type(const std::string & str);

private:
	const unsigned long int id_;			// IDは絶対に不変
	std::wstring text_;
	std::wstring detail_text_;
	Type type_;
	std::wstring regex_rule_;
	std::vector<std::wstring> choise_;
	unsigned int length_;
	Phase required_when_phase_;
	std::chrono::system_clock::time_point required_when_datetime_;
	bool multiline_;
	bool is_required_;
};



}
}

#endif