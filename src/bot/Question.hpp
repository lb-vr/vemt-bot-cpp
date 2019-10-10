#ifndef VEMT_QUESTION_HPP
#define VEMT_QUESTION_HPP

#include "db/QuestionItemModel.hpp"
#include <string>
#include <vector>

namespace vemt {
namespace bot {

class Question {
public:
	Question(
		const std::string & author,
		const std::string & title,
		const std::vector<std::string> & headers,
		const std::vector<QuestionItemModel> & question_items
	) noexcept;

	~Question() noexcept;

	static Question loadFromJson(const std::string & json_str, std::string & error_message);

private:
	const std::string author_;
	const std::string title_;
	const std::vector<std::string> headers_;
	const std::vector<QuestionItemModel> question_items_;
};

}
}

#endif