#include "Question.hpp"
#include "json11.hpp"
#include <unordered_map>

vemt::bot::Question::Question(const std::string & author, const std::string & title, const std::string & headers, const std::vector<QuestionItemModel>& question_items) noexcept
	: author_(author), title_(title), headers_(headers), question_items_(question_items) {}

vemt::bot::Question::~Question() noexcept {}

const std::string & vemt::bot::Question::getTitle() const { return this->title_; }
const std::string & vemt::bot::Question::getHeader() const { return this->headers_; }
const std::vector<vemt::bot::QuestionItemModel> & vemt::bot::Question::getQuestionItem() const { return this->question_items_; }


vemt::bot::Question vemt::bot::Question::loadFromJson(const std::string & json_str, std::string & error_message) {
	error_message = "";

	
	//### Parse
	auto json = json11::Json::parse(json_str, error_message);
	if (!error_message.empty()) {
		return Question("", "", {}, {});
	}

	//### read
	auto author = json["author"].asString("");
	auto title = json["title"].asString("");
	std::string header = "";
	if (json["detail"].is_array()) {
		auto details_json = json["detail"].arrayItems();
		for (const auto & dt : details_json) {
			auto line = dt.asString("");
			if (!line.empty()) header += line + "\n";
		}
		header.pop_back();
	}
	// error check
	if (author.empty()) {
		error_message = "\"author\" is empty.";
		return Question("", "", {}, {});
	}
	if (title.empty() && header.empty()) {
		error_message = "\"title\" and \"details\" are empty.";
		return Question("", "", {}, {});
	}
	
	//### QuestionItemModel
	const auto kTypeMapping = std::unordered_map<std::string, vemt::bot::QuestionItemModel::Type>({
		{"string", QuestionItemModel::kString},
		{"number", QuestionItemModel::kNumber},
		{"picture", QuestionItemModel::kPicture},
		{"jsonfile", QuestionItemModel::kJsonFile},
		{"json", QuestionItemModel::kJson},
		{"regex", QuestionItemModel::kRegex}});

	auto items = std::vector<QuestionItemModel>();
	if (json["items"].is_array()) {
		auto items_json = json["item"].arrayItems();
		for (const auto & itm : items_json) {
			items.emplace_back(QuestionItemModel::createFromJson(itm, error_message));
			if (!error_message.empty()) return Question("", "", {}, {});
		}
	}
	else {
		error_message = "invalid \"items\".";
		return Question("", "", {}, {});
	}
	if (items.empty()) {
		error_message = "\"Items\" is empty.";
		return Question("", "", {}, {});
	}

	error_message = "";
	return Question(author, title, header, items);
}
