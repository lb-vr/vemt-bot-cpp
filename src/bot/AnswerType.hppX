#ifndef VEMT_ANSWERTYPE_HPP
#define VEMT_ANSWERTYPE_HPP

#include <stdexcept>
#include <string>

namespace vemt {
namespace bot {

class AnswerType {
public:

	class ParseError : public std::invalid_argument {
	public:
		explicit ParseError(const std::string & str) noexcept;
	};

	static const AnswerType kString;
	static const AnswerType kNumber;
	static const AnswerType kPicture;
	static const AnswerType kJsonFile;
	static const AnswerType kJson;
	static const AnswerType kRegex;

	AnswerType(const int type_value) noexcept;
	AnswerType(const AnswerType & cp) noexcept;
	~AnswerType() noexcept;

	std::wstring validate(const std::wstring & answer_str, const std::wstring & regex_rule = L".+") const;

	const std::string toString(void) const;
	const std::wstring toWString(void) const;
	const int getAsInt(void) const;
	void setFromInt(const int new_type_value);
	void set(const AnswerType & new_type);

	const AnswerType & operator=(const int new_type_value);
	const AnswerType & operator=(const AnswerType & new_type);

	const bool operator==(const int type) const;
	const bool operator==(const AnswerType & type) const;

	operator int() const;
	operator std::string() const;
	operator std::wstring() const;

	/// @brief 文字列からAnswerTypeを生成する
	/// @throw ParseError 不正な文字列だった場合
	static AnswerType parseFromString(const std::string & str);

	/// @brief 数値が範囲内かチェックする
	static bool isValidType(const int type);

private:
	int type_value_;
};

}
}

#endif