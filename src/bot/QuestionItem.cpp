#include "QuestionItem.hpp"
#include "util/json11.hpp"
#include "util/Logger.hpp"
#include "util/string_util.hpp"
#include <chrono>

vemt::bot::QuestionItem::QuestionItem(
	const std::wstring & text, const std::wstring & detail_text,
	const type::AnswerType type, const std::wstring & regex_rule, const std::vector<std::wstring> choise,
	const int & length, const bool is_required, const type::PhaseParam required_when_phase,
	const type::DatetimeParam & required_when_datetime, const bool & multiline) noexcept
	: QuestionItemModel(
		text, detail_text, type, regex_rule, QuestionItem::_toWstringVector(choise), length, is_required,
		required_when_phase, required_when_datetime, multiline) {}

vemt::bot::QuestionItem::QuestionItem(const db::QuestionItemModel & model) noexcept
	: QuestionItemModel(model) {}

vemt::bot::QuestionItem::~QuestionItem() {}

vemt::bot::QuestionItem::ValidationResult vemt::bot::QuestionItem::validate(const std::wstring & answer) const {
	return ValidationResult::kOk;	// TODO: まだ実装していない
}

std::string vemt::bot::QuestionItem::toString() const {
	return std::string("QuestionItem instance.");
}

std::wstring vemt::bot::QuestionItem::createFullMessage() const {
	std::wstring wstr;
	auto phase_param = this->getRequiredWhenPhase();
	wstr += L"**Q" + std::to_wstring(this->getId().get()) + L". ";
	if (this->getIsRequired().get()) wstr += L" 【必須】 ";
	wstr += this->getText().get() + L"**\\n";
	if (!this->getDetailText().get().empty()) wstr += this->getDetailText().get() + L"\\n";
	if (!this->getChoise().empty()) {
		wstr += L"選択肢 : [";
		bool first = true;
		for (const auto c : this->getChoise()) {
			if (!first) wstr += L" | ";
			else first = false;
			wstr += c.get();
		}
		wstr += L"]\\n";
	}
	wstr += L"< " + this->getType().toDisplayWstring() + L" | ";
	wstr += L"回答・編集期限 : " + util::widen(this->getRequireWhenDatetime().toString()) + L" / ";
	wstr += phase_param.toDisplayWstring() + L" >\\n";
	wstr += L"    TODO : <未回答>\\n"; // TODO
	wstr += L"\\n";
	return wstr;
}

vemt::bot::QuestionItem vemt::bot::QuestionItem::loadFromJson(const json11::Json & json) {
	logging::debug << " - QuestionItem::loadFromJson" << std::endl;

	// text
	auto text = util::replaceW(json["text"].asWString(L""), std::wstring(L"\n"), std::wstring(L""));
	if (text.length() > 200) throw JsonParseError(L"textの文字列が長すぎます。200文字以内にしてください。");
	logging::debug << " - - text = " << util::narrow(text) << std::endl;

	// detail_text
	auto detail_str = std::wstring();
	if (json["details"].is_array()) {
		for (const auto & detail : json["details"].arrayItems()) {
			auto line = detail.asWString(L"");
			if (!line.empty()) {
				if (!detail_str.empty()) detail_str += L"\\n";
				detail_str += (util::replaceW(line, std::wstring(L"\n"), std::wstring(L"")));
			}
		}
	}
	if (detail_str.length() > 1024) throw JsonParseError(L"detailの文字が長すぎます。1024文字以内にしてください。");
	logging::debug << " - - text = " << util::narrow(detail_str) << std::endl;

	// type
	auto type = type::AnswerTypeParam(type::AnswerType::kString);
	try {
		type = type::AnswerTypeParam::parseFromString(json["type"].asString("string"));
	}
	catch (const type::AnswerTypeParam::ParseException & e) {
		throw JsonParseError(L"typeの値が不正です。詳しくは`+config help question`を参照してください。");
	}
	logging::debug << " - - type = " << type.toString() << std::endl;
	
	// regex
	auto wregex_rule = json["regex_rule"].asWString(L".+");
	try {
		std::wregex(wregex_rule);
	}
	catch (std::regex_error e) {
		throw JsonParseError(L"正規表現のルール記述が間違っています。" + util::widen(e.what()));
	}
	logging::debug << " - - regex_rule = " << util::narrow(wregex_rule) << std::endl;

	// choise
	auto choise = std::vector<std::wstring>();
	if (json["choise"].is_array()) {
		for (const auto & choise_json : json["choise"].arrayItems()) {
			auto item = choise_json.asWString(L"");
			if (!item.empty()) choise.push_back(util::replaceW(item, std::wstring(L"\n"), std::wstring(L"")));
			logging::debug << " - - choise [" << choise.size() << "] : " << util::narrow(item) << std::endl;
		}
	}

	// length
	unsigned int length = json["length"].asInt(128);
	if (length > 500) {
		throw JsonParseError(L"字数制限は500文字以内にする必要があります。");
	}
	logging::debug << " - - length = " << length << std::endl;

	// required
	auto is_required = json["is_required"].asBool(false);
	logging::debug << " - - is_required = " << (is_required ? "true" : "false") << std::endl;

	auto required_when_phase = type::Phase::kEntry; // json["required_when_phase"].asInt(Phase::kPublish.to_int());
	
	auto required_when_datetime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());	// TODO: jsonから指定

	// multiline
	auto multiline = json["multiline"].asBool(false);
	logging::debug << " - - multiline = " << (multiline ? "true" : "false") << std::endl;

	logging::debug << " - - Finished create QuestionItem." << std::endl;
		
	return QuestionItem(text, detail_str, type.get(), wregex_rule, choise, length, is_required, required_when_phase, required_when_datetime, multiline);
}

std::vector<vemt::type::WstringParam> vemt::bot::QuestionItem::_toWstringVector(const std::vector<std::wstring>& wvec) {
	std::vector<type::WstringParam> ret;
	for (const auto & w : wvec) {
		ret.push_back(w);
	}
	return ret;
}

vemt::bot::JsonParseError::JsonParseError(const std::wstring & str) noexcept
	: std::invalid_argument(util::narrow(str).c_str()) {
	logging::warn << "Failed to parse. Message = " << util::narrow(str) << std::endl;
}