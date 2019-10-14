#include "AnswerType.hpp"
#include "Logger.hpp"
#include "json11.hpp"
#include "util/string_util.hpp"
#include <regex>
#include <fstream>
#include <array>

const vemt::bot::AnswerType vemt::bot::AnswerType::kString(1);
const vemt::bot::AnswerType vemt::bot::AnswerType::kNumber(2);
const vemt::bot::AnswerType vemt::bot::AnswerType::kPicture(3);
const vemt::bot::AnswerType vemt::bot::AnswerType::kJsonFile(4);
const vemt::bot::AnswerType vemt::bot::AnswerType::kJson(5);
const vemt::bot::AnswerType vemt::bot::AnswerType::kRegex(6);

vemt::bot::AnswerType::AnswerType(const int type_value) noexcept
	: type_value_(type_value) {}

vemt::bot::AnswerType::AnswerType(const AnswerType & cp) noexcept
	: type_value_(cp.type_value_) {}

vemt::bot::AnswerType::~AnswerType() noexcept {}

namespace {
bool isPngFile(const std::wstring & filepath) {
	logging::info << "Start isPngFile validation. Filepath = " << vemt::util::narrow(filepath) << std::endl;

	// -- Signature
	std::ifstream ifst(filepath, std::ios::binary | std::ios::in);

	// file open
	if (!ifst) {
		logging::warn << " - Failed to validate. File not found. Filepath = " << vemt::util::narrow(filepath) << std::endl;
		return false;
	}
	
	// signature validation
	std::array<char, 8> buf;
	const std::array<char, 8> check = { 0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n' };
	ifst.read(buf.data(), 8);
	for (int i = 0; i < 8; i++) {
		if (buf.at(i) != check.at(i)) {
			logging::warn << " - Failed to validate. Invalid PNG signature. ["
				<< i + 1 << "] byte must be " << static_cast<int>(check.at(i)) << " not " << static_cast<int>(buf.at(i));
			return false;
		}
	}
	logging::info << " - Valid PNG Data. Filepath = " << vemt::util::narrow(filepath) << std::endl;
	return true;
}

bool isJpegFile(const std::wstring & filepath) {
	logging::info << "Start isJpgFile validation. Filepath = " << vemt::util::narrow(filepath) << std::endl;

	// SOI / EOI
	std::ifstream ifst(filepath, std::ios::binary | std::ios::in);
	
	// file open
	if (!ifst) {
		logging::warn << " - Failed to validate. File not found. Filepath = " << vemt::util::narrow(filepath) << std::endl;
		return false;
	}

	// soi / eoi
	std::array<char, 2> buf;
	ifst.read(buf.data(), 2);
	if (buf.at(0) != 0xFF || buf.at(1) != 0xD8) {
		logging::warn << " - Failed to validate. Invalid SOI data. [0, 1] must be (255, 216) not ("
			<< static_cast<int>(buf.at(0)) << ", " << static_cast<int>(buf.at(1)) << ")" << std::endl;
		return false;
	}

	ifst.seekg(ifst.end - 2);
	if (!ifst) {
		logging::warn << " - Failed to validate. Failed to seek." << std::endl;
		return false;
	}

	ifst.read(buf.data(), 2);
	if (buf.at(0) != 0xFF || buf.at(1) != 0xD9) {
		logging::warn << " - Failed to validate. Invalid EOI data. [0, 1] must be (255, 217) not ("
			<< static_cast<int>(buf.at(0)) << ", " << static_cast<int>(buf.at(1)) << ")" << std::endl;
		return false;
	}
	return true;
}

bool isJsonFile(const std::wstring & filepath) {
	logging::info << "Start isJsonFile validation. Filepath = " << vemt::util::narrow(filepath) << std::endl;
	std::ifstream ifst(filepath, std::ios::in);

	// file open
	if (!ifst) {
		logging::warn << " - Failed to validate. File not found. Filepath = " << vemt::util::narrow(filepath) << std::endl;
		return false;
	}

	std::string error_str;
	json11::Json::parse(vemt::util::narrow(filepath), error_str);
	if (!error_str.empty()) {
		logging::warn << " - Failed to validate. Invalid formatted. Message = " << error_str << std::endl;
		return false;
	}
	return true;
}

class ValidationError : public std::exception {
private:
	std::wstring error_msg_;
public:
	explicit ValidationError(const std::wstring & message)
		: std::exception(vemt::util::narrow(message).c_str()), error_msg_(message) {
		logging::warn << "Failed Type validation. Message = " << vemt::util::narrow(message) << std::endl;
	}
	~ValidationError() {}
	std::wstring getErrorMsg() const { return this->error_msg_; }
};
}

std::wstring vemt::bot::AnswerType::validate(const std::wstring & answer_str, const std::wstring & regex_rule) const {
	try {
		if (*this == this->kString) {
			if (answer_str.empty()) throw ValidationError(L"文字列が空です。");
		}
		else if (*this == this->kNumber) {
			try { std::stod(answer_str); }
			catch (std::invalid_argument) {
				throw ValidationError(L"数値ではありません。");
			}
		}
		else if (*this == this->kPicture) {
			std::wregex wre_jpg(L".+\.(jpg|JPG|jpeg|JPEG)");
			std::wregex wre_png(L".+\.(png|PNG)");
			if (std::regex_match(answer_str, wre_jpg)) {
				if (isJpegFile(answer_str)) throw ValidationError(L"不正なJPEGファイルです。");
			}
			else if (std::regex_match(answer_str, wre_png)) {
				if (isPngFile(answer_str)) throw ValidationError(L"不正なPNGファイルです。");
			}
			else throw ValidationError(L"画像はPNG/JPEGのみ使用可能です。");
		}
		else if (*this == this->kJsonFile) {
			std::wregex wre(L".+\.(json|JSON)");
			if (!std::regex_match(answer_str, wre)) throw ValidationError(L"JSONファイルを指定してください。");
			else if (!isJsonFile(answer_str)) throw ValidationError(L"不正なJSONファイルです。");
		}
		else if (*this == this->kJson) {
			std::string error_str;
			json11::Json::parse(vemt::util::narrow(answer_str), error_str);
			if (!error_str.empty()) throw ValidationError(L"不正なJSON文字列です。");
		}
		else if (*this == this->kRegex) {
			try {
				std::wregex wre(regex_rule);
				if (!std::regex_match(answer_str, wre)) throw ValidationError(L"回答は正規表現にマッチしませんでした。");
			}
			catch (std::regex_error) {
				throw ValidationError(L"不正な正規表現です。/// 管理者へ連絡してください。 ///");
			}
		}
	}
	catch (const ValidationError & e) {
		return e.getErrorMsg();
	}
	return L"";
}

const std::string vemt::bot::AnswerType::toString(void) const {
	if (*this == this->kString) return "String";
	else if (*this == this->kNumber) return "Number";
	else if (*this == this->kPicture) return "Picture";
	else if (*this == this->kJsonFile) return "JsonFile";
	else if (*this == this->kJson) return "Json";
	else if (*this == this->kRegex) return "Regex";
	return std::string();
}

const std::wstring vemt::bot::AnswerType::toWString(void) const
{
	if (*this == this->kString) return L"文字列";
	else if (*this == this->kNumber) return L"数値";
	else if (*this == this->kPicture) return L"JPG/PNG画像";
	else if (*this == this->kJsonFile) return L"Jsonファイル";
	else if (*this == this->kJson) return L"Json文字列";
	else if (*this == this->kRegex) return L"正規表現制約付き文字列";
	return std::wstring();
}

const int vemt::bot::AnswerType::getAsInt(void) const { return this->type_value_; }
void vemt::bot::AnswerType::setFromInt(const int new_type_value) { this->type_value_ = new_type_value; }
void vemt::bot::AnswerType::set(const AnswerType & new_type) { this->setFromInt(new_type.getAsInt()); }
const vemt::bot::AnswerType & vemt::bot::AnswerType::operator=(const int new_type_value) {
	this->setFromInt(new_type_value);
	return *this;
}
const vemt::bot::AnswerType & vemt::bot::AnswerType::operator=(const AnswerType & new_type) {
	this->set(new_type);
	return *this;
}
const bool vemt::bot::AnswerType::operator==(const int type) const {
	return this->getAsInt() == type;
}
const bool vemt::bot::AnswerType::operator==(const AnswerType & type) const {
	return *this == type.getAsInt();
}
vemt::bot::AnswerType::operator int() const { return this->getAsInt(); }
vemt::bot::AnswerType::operator std::string() const { return this->toString(); }
vemt::bot::AnswerType::operator std::wstring() const { return this->toWString(); }

