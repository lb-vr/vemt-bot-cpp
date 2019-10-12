#include "CodeCvt.hpp"
#include <stdexcept>
#include <vector>

typedef std::codecvt<wchar_t, char, mbstate_t> codecvt_type;

namespace {
void assert_result(const std::codecvt_base::result & res, const void * const fn, const void * const fe) {
	switch (res) {
	case std::codecvt_base::ok:
		if (fn != fe) throw std::invalid_argument("Conversion failure");
		break;
	case std::codecvt_base::partial:
		throw std::invalid_argument("partial");
	case std::codecvt_base::error:
		throw std::invalid_argument("error");
	case std::codecvt_base::noconv:
		throw std::invalid_argument("noconv");
	}
}
}

CodeCvt::CodeCvt(const std::string & locale)
	: CodeCvt(std::locale(locale)) {}

CodeCvt::CodeCvt(const std::locale & locale) noexcept
	: locale_(locale), codecvt_(std::use_facet<codecvt_type>(locale)) {}

std::wstring CodeCvt::widen(const std::string & s) const {
	const char *fb = s.data();
	const char *fe = fb + s.size();
	const char *fn;
	std::vector<wchar_t> ws(s.size());
	wchar_t *tb = &ws.front();
	wchar_t *te = tb + ws.size();
	wchar_t *tn;
	mbstate_t st = { 0 };
	// bzero(&st, sizeof(st));

	const std::codecvt_base::result res = codecvt_.in(st, fb, fe, fn, tb, te, tn);
	assert_result(res, fn, fe);
	return std::wstring(tb, tn);
}

std::wstring CodeCvt::widen(const std::string & s, std::string * const error_msg) const noexcept {
	std::wstring ret;
	try { ret = this->widen(s); }
	catch (std::invalid_argument e) {
		if(error_msg != nullptr) *error_msg = e.what();
	}
	return ret;
}

std::string CodeCvt::narrow(const std::wstring & ws) const {
	const wchar_t *fb = ws.data();
	const wchar_t *fe = fb + ws.size();
	const wchar_t *fn;
	std::vector<char> s(ws.size() * 4);
	char *tb = &s.front();
	char *te = tb + s.size();
	char *tn;
	mbstate_t st = { 0 };
	// bzero(&st, sizeof(st));

	const std::codecvt_base::result res = codecvt_.out(st, fb, fe, fn, tb, te, tn);
	assert_result(res, fn, fe);
	return std::string(tb, tn);
}

std::string CodeCvt::narrow(const std::wstring & ws, std::string * const error_msg) const noexcept {
	std::string ret;
	try { ret = this->narrow(ws); }
	catch (std::invalid_argument e) {
		if (error_msg != nullptr) *error_msg = e.what();
	}
	return ret;
}
