#include "QuestionItem.hpp"
#include "util/json11.hpp"
#include "util/Logger.hpp"
#include "util/string_util.hpp"

vemt::bot::QuestionItem::QuestionItem(
	const std::wstring & text, const std::wstring & detail_text,
	const db::type::AnswerType type, const std::wstring & regex_rule, const std::vector<std::wstring> choise,
	const int & length, const bool is_required, const Phase required_when_phase,
	const db::type::DatetimeParam & required_when_datetime, const bool & multiline) noexcept
	: QuestionItemModel(
		text, detail_text, db::type::AnswerTypeParam(type), regex_rule, QuestionItem::_toWstringVector(choise), length, is_required,
		required_when_phase.to_int(), required_when_datetime, multiline) {}

vemt::bot::QuestionItem::QuestionItem(const db::QuestionItemModel & model) noexcept
	: QuestionItemModel(model) {}

vemt::bot::QuestionItem::~QuestionItem() {}

vemt::bot::QuestionItem::ValidationResult vemt::bot::QuestionItem::validate(const std::wstring & answer) const {
	return ValidationResult::kOk;	// TODO: まだ実装していない
}

std::string vemt::bot::QuestionItem::toString() const {
	return std::string("QuestionItem instance.");
}

vemt::bot::QuestionItem vemt::bot::QuestionItem::createFromJson(const json11::Json & json, std::string & error_msg) {
	// text
	auto text = util::replaceW(json["text"].asWString(L""), std::wstring(L"\n"), std::wstring(L""));
	if (text.length() > 200) throw JsonParseError(L"textの文字列が長すぎます。200文字以内にしてください。");

	// detail_text
	auto detail_str = std::wstring();
	if (json["details"].is_array()) {
		for (const auto & detail : json["details"].arrayItems()) {
			auto line = detail.asWString(L"");
			if (!line.empty()) detail_str += (util::replaceW(line, std::wstring(L"\n"), std::wstring(L"")) + L"\\n");
		}
	}
	if (detail_str.length() > 1000) throw JsonParseError(L"detailの文字が長すぎます。1000文字以内にしてください。");

	// type
	auto type = db::type::AnswerTypeParam(db::type::AnswerType::kString);
	try {
		type = db::type::AnswerTypeParam::parseFromString(json["type"].asString("string"));
	}
	catch (const db::type::AnswerTypeParam::ParseError & e) {
		throw JsonParseError(L"typeの値が不正です。詳しくは`+config help question`を参照してください。");
	}
	
	// regex
	auto wregex_rule = json["regex_rule"].asWString(L".+");
	try {
		std::wregex(wregex_rule);
	}
	catch (std::regex_error e) {
		throw JsonParseError(L"正規表現のルール記述が間違っています。" + util::widen(e.what()));
	}

	// choise
	auto choise = std::vector<std::wstring>();
	if (json["choise"].is_array()) {
		for (const auto & choise_json : json["choise"].arrayItems()) {
			auto item = choise_json.asWString(L"");
			if (!item.empty()) choise.push_back(util::replaceW(item, std::wstring(L"\n"), std::wstring(L"")));
		}
	}

	// length
	unsigned int length = json["length"].asInt(128);
	if (length > 500) {
		throw JsonParseError(L"字数制限は500文字以内にする必要があります。");
	}

	// required
	auto is_required = json["is_required"].asBool(false);
	

	auto required_when_phase = Phase::kEntry; // json["required_when_phase"].asInt(Phase::kPublish.to_int());
	
	auto required_when_datetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());	// TODO: jsonから指定

	// multiline
	auto multiline = json["multiline"].asBool(false);
		
	return QuestionItem(text, detail_str, type.get(), wregex_rule, choise, length, is_required, required_when_phase, required_when_datetime, multiline);
}

std::vector<vemt::db::type::WstringParam> vemt::bot::QuestionItem::_toWstringVector(const std::vector<std::wstring>& wvec) {
	std::vector<db::type::WstringParam> ret;
	for (const auto & w : wvec) {
		ret.push_back(w);
	}
	return ret;
}

vemt::bot::QuestionItem::JsonParseError::JsonParseError(const std::wstring & str) noexcept
	: std::invalid_argument(util::narrow(str).c_str()) {
	logging::warn << "Failed to parse. Message = " << util::narrow(str) << std::endl;
}