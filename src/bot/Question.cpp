#include "Question.hpp"
#include "util/json11.hpp"
#include <unordered_map>
#include "util/string_util.hpp"

#include "db/QuestionItemsTable.hpp"

vemt::bot::Question::Question(void) noexcept : Question(L"", L"", L"", {}) {}

vemt::bot::Question::Question(const std::wstring & author, const std::wstring & title, const std::wstring & headers, const std::vector<QuestionItem> & question_items) noexcept
	: author_(author), title_(title), headers_(headers), question_items_(question_items) {}

vemt::bot::Question::~Question() noexcept {}

const std::wstring & vemt::bot::Question::getTitle() const { return this->title_; }
const std::wstring & vemt::bot::Question::getHeader() const { return this->headers_; }
const std::vector<vemt::bot::QuestionItem> & vemt::bot::Question::getQuestionItem() const { return this->question_items_; }

const std::wstring vemt::bot::Question::createAsQuestionMessage() const
{
	std::wstring wstr = L"**" + this->getTitle() + L"**\\n";
	wstr += this->getHeader() + L"\\n";
	for (const auto & p : this->getQuestionItem()) {
		wstr += p.createFullMessage();
	}
	return wstr;
}

vemt::bot::Question vemt::bot::Question::loadFromJson(const std::string & json_str) {
	std::string error_msg = "";
	
	//### Parse
	auto json = json11::Json::parse(json_str, error_msg);
	if (!error_msg.empty()) {
		throw JsonParseError(L"JSONファイルの構文に間違いがあります。" + util::widen(error_msg));
	}

	//### read
	auto author = json["author"].asWString(L"");
	auto title = json["title"].asWString(L"");
	std::wstring header;
	if (json["descriptions"].is_array()) {
		auto details_json = json["descriptions"].arrayItems();
		for (const auto & dt : details_json) {
			auto line = dt.asWString(L"");
			if (!line.empty()) {
				if (!header.empty()) header += L"\\n";
				header += line;
			}
		}
	}
	// error check
	if (author.empty()) throw JsonParseError(L"`author`が存在しないか空文字です。");
	if (author.size() > 64) throw JsonParseError(L"`author`が長すぎます。64文字以内にしてください。");
	if (title.empty()) throw JsonParseError(L"`title`が存在しないか空文字です。");
	if (title.size() > 100) throw JsonParseError(L"`title`が長すぎます。100文字以内にしてください。");
	if (header.size() > 1024) throw JsonParseError(L"`description`が長すぎます。1024文字以内にしてください。");
	
	//### QuestionItemModel
	auto items = std::vector<QuestionItem>();
	if (json["items"].is_array()) {
		auto items_json = json["items"].arrayItems();
		for (const auto & itm : items_json) {
			items.emplace_back(QuestionItem::loadFromJson(itm));
		}
	}
	else {
		throw JsonParseError(L"`items`が存在しないか、配列ではありません。");
	}
	return Question(author, title, header, items);
}

vemt::bot::Question vemt::bot::Question::loadFromDatabase(const std::string & database_path) {
	// TODO: ここでタイトルやヘッダを取得

	auto qitems_m = db::QuestionItemsTable(database_path).getAll();
	std::vector<QuestionItem> qitems;
	for (const auto & p : qitems_m) {
		qitems.emplace_back(p);
	}

	return Question(L"Dummy Author", L"Dummy Title", L"Dummy Header.\\nDummy Header2.", qitems);
}
