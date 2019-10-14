#ifndef VEMT_ANSWERTYPE_HPP
#define VEMT_ANSWERTYPE_HPP

#include <string>

namespace vemt {
namespace bot {

class AnswerType {
public:
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

private:
	int type_value_;
};

}
}

#endif