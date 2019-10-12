#include <iostream>
#include <string>
#include <cstring>
#include <locale>

class CodeCvt {
public:
	explicit CodeCvt(const std::string & locale = "ja_JP.UTF-8") noexcept;
	explicit CodeCvt(const std::locale & locale) noexcept;

	/// @brief ワイド文字列への変換
	/// @throw std::invalid_argument 変換に失敗した場合
	std::wstring widen(const std::string & s) const;

	/// @brief ワイド文字列への変換
	///
	/// 例外を投げずに、エラー文字列として返す。
	std::wstring widen(const std::string & s, std::string * const error_msg) const noexcept;

	// @brief マルチバイト文字列への変換
	// @throw std::invalid_argument 変換に失敗した場合
	std::string narrow(const std::wstring & ws) const;

	std::string narrow(const std::wstring & ws, std::string * const error_msg) const noexcept;

private:
	const std::locale locale_;
	const codecvt_type & codecvt_;
};
