#ifndef VEMT_QUESTION_HPP
#define VEMT_QUESTION_HPP

#include "QuestionItem.hpp"
#include <string>
#include <vector>

namespace vemt {
namespace bot {

class Question {
public:
	Question(
		const std::wstring & author,
		const std::wstring & title,
		const std::wstring & header,
		const std::vector<QuestionItem> & question_items
	) noexcept;

	~Question() noexcept;

	const std::wstring & getTitle() const;
	const std::wstring & getHeader() const;
	const std::vector<QuestionItem> & getQuestionItem() const;

	const std::wstring createAsQuestionMessage() const;

	static Question loadFromJson(const std::string & json_str);

private:
	Question() noexcept;

	const std::wstring author_;
	const std::wstring title_;
	const std::wstring headers_;
	const std::vector<QuestionItem> question_items_;
};

}
}

#endif