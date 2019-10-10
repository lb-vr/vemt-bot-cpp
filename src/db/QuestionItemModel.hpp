#ifndef VEMT_QUESTIONITEMMODEL_HPP
#define VEMT_QUESTIONITEMMODEL_HPP

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

class QuestionItemModel {
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
	QuestionItemModel(
		const unsigned long int id,
		const std::string & text,
		const std::string & detail_text,
		const Type type,
		const std::string & regex_rule,
		const std::vector<std::string> & choise,
		const unsigned int length,
		const bool is_required,
		const Phase required_when_phase,
		const std::chrono::system_clock::time_point required_when_datetime,
		const bool multiline
	) noexcept;

	/// @brief Jsonパース用のコンストラクタ
	///
	/// idについてはダミーを入れる？
	QuestionItemModel(
		const std::string & text,
		const std::string & detail_text,
		const Type type,
		const std::string & regex_rule,
		const std::vector<std::string> & choise,
		const unsigned int length,
		const bool is_required,
		const Phase required_when_phase,
		const std::chrono::system_clock::time_point required_when_datetime,
		const bool multiline
	) noexcept;

	/// @brief コピーコンストラクタ
	QuestionItemModel(const QuestionItemModel & copy) noexcept;

	/// @brief デストラクタ
	~QuestionItemModel();

	/// @brief 答えの文字列に対してバリデーティングをする
	ValidationResult validate(const std::string & answer) const;

	// -- Get Accessor --
	const unsigned long int getId() const;
	const std::string & getText() const;
	const std::string & getDetailText() const;
	const Type getType() const;
	const std::regex getRegex() const;
	const std::string getRegexRule() const;
	const std::vector<std::string> & getChoise() const;
	const unsigned int getLength() const;
	const bool getIsRequired() const;
	const Phase getRequiredWhenPhase() const;
	const std::chrono::system_clock::time_point getRequireWhenDatetime() const;
	const bool getMultiline() const;
	const std::chrono::system_clock::time_point getCreatedAt() const;

	// -- Set Accessor --
	void setText(const std::string & text);
	void setDetailText(const std::string & detail_text);
	void setType(const Type type);
	void setRegexRule(const std::string & regex_rule);
	void setChoise(const std::vector<std::string> & choise);
	void setLength(const unsigned int length);
	void setIsRequired(const bool required);
	void setRequiredWhenPhase(const Phase phase);
	void setRequiredWhenDatetime(const std::chrono::system_clock::time_point & timepoint);
	void setMultiline(const bool multiline);

	// -- utility --
	std::string toString() const;

	// -- parser ---
	static QuestionItemModel createFromJson(const json11::Json & json, std::string & error_msg);

	// -- caster --
	static std::string type2str(const Type type);
	static Type str2type(const std::string & str);

private:
	const unsigned long int id_;			// IDは絶対に不変
	std::string text_;
	std::string detail_text_;
	Type type_;
	std::string regex_rule_;
	std::vector<std::string> choise_;
	unsigned int length_;
	Phase required_when_phase_;
	std::chrono::system_clock::time_point required_when_datetime_;
	bool multiline_;
	bool is_required_;
	std::chrono::system_clock::time_point created_at_;
};

}
}

#endif