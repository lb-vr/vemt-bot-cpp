#include "Question.hpp"
#include "json11/src/json11.hpp"
#include <unordered_map>

vemt::bot::Question::Question(const std::string & author, const std::string & title, const std::vector<std::string>& headers, const std::vector<QuestionItem>& question_items) noexcept
	: author_(author), title_(title), headers_(headers), question_items_(question_items) {}

vemt::bot::Question::~Question() noexcept {}

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
	std::vector<std::string> header;
	if (json["detail"].is_array()) {
		auto details_json = json["detail"].arrayItems();
		for (const auto & dt : details_json) {
			auto line = dt.asString("");
			if (!line.empty()) header.push_back(line);
		}
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
	
	//### QuestionItem
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
			auto text = itm["text"].asString("");
			auto type_str = itm["type"].asString("string");
			if (!kTypeMapping.count(type_str)) {
				error_message = "Invalid \"type\".";
				return Question("", "", {}, {});
			}
			auto len = itm["len"].asInt(1024);
			auto optional = itm["optional"].asBool(true);
			auto multiline = itm["multiline"].asBool(false);
			auto regex = itm["regex"].asString(".+");
			auto details = std::vector<std::string>();
			if (itm["details"].is_array()) {
				auto details_json = itm["details"].arrayItems();
				for (const auto & dt : details_json) {
					auto line = dt.asString("");
					if(!line.empty()) details.push_back(line);
				}
			}
			auto choise = std::vector<std::string>();
			if (itm["choise"].is_array()) {
				auto choise_json = itm["choise"].arrayItems();
				for (const auto & ch : choise_json) {
					auto line = ch.asString("");
					if (!line.empty()) choise.push_back(line);
				}
			}

			// error check
			if (text.empty()) {
				error_message = "\"text\" in items is empty.";
				return Question("", "", {}, {});
			}
			items.emplace_back(QuestionItem(text, details, kTypeMapping.at(type_str), regex, choise, len, optional, multiline));
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
