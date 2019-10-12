#include "Question.hpp"
#include "json11.hpp"
#include <unordered_map>

vemt::bot::Question::Question(void) noexcept : Question(L"", L"", L"", {}) {}

vemt::bot::Question::Question(const std::wstring & author, const std::wstring & title, const std::wstring & headers, const std::vector<QuestionItem> & question_items) noexcept
	: author_(author), title_(title), headers_(headers), question_items_(question_items) {}

vemt::bot::Question::~Question() noexcept {}

const std::wstring & vemt::bot::Question::getTitle() const { return this->title_; }
const std::wstring & vemt::bot::Question::getHeader() const { return this->headers_; }
const std::vector<vemt::bot::QuestionItem> & vemt::bot::Question::getQuestionItem() const { return this->question_items_; }


vemt::bot::Question vemt::bot::Question::loadFromJson(const std::string & json_str, std::string & error_message) {
	error_message = "";

	
	//### Parse
	auto json = json11::Json::parse(json_str, error_message);
	if (!error_message.empty()) {
		return Question();
	}

	//### read
	auto author = json["author"].asWString(L"");
	auto title = json["title"].asWString(L"");
	std::wstring header;
	if (json["detail"].is_array()) {
		auto details_json = json["detail"].arrayItems();
		for (const auto & dt : details_json) {
			auto line = dt.asWString(L"");
			if (!line.empty()) header += line + L"\n";
		}
		header.pop_back();
	}
	// error check
	if (author.empty()) {
		error_message = "\"author\" is empty.";
		return Question();
	}
	if (title.empty() && header.empty()) {
		error_message = "\"title\" and \"details\" are empty.";
		return Question();
	}
	
	//### QuestionItemModel
	const auto kTypeMapping = std::unordered_map<std::string, vemt::bot::QuestionItem::Type>({
		{"string", QuestionItem::kString},
		{"number", QuestionItem::kNumber},
		{"picture", QuestionItem::kPicture},
		{"jsonfile", QuestionItem::kJsonFile},
		{"json", QuestionItem::kJson},
		{"regex", QuestionItem::kRegex}});

	auto items = std::vector<QuestionItem>();
	if (json["items"].is_array()) {
		auto items_json = json["item"].arrayItems();
		for (const auto & itm : items_json) {
			items.emplace_back(QuestionItem::createFromJson(itm, error_message));
			if (!error_message.empty()) return Question();
		}
	}
	else {
		error_message = "invalid \"items\".";
		return Question();
	}
	if (items.empty()) {
		error_message = "\"Items\" is empty.";
		return Question();
	}

	error_message = "";
	return Question(author, title, header, items);
}
