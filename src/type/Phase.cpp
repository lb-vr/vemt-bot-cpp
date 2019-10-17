#include "Phase.hpp"
#include "util/Logger.hpp"

vemt::type::PhaseParam::PhaseParam() noexcept : Param() {}
vemt::type::PhaseParam::PhaseParam(const Phase value) : Param() { this->set(value); }
vemt::type::PhaseParam::PhaseParam(const PhaseParam & phase_param) : Param() { *this = phase_param; }
vemt::type::PhaseParam & vemt::type::PhaseParam::operator=(const PhaseParam & phase_param) {
	if (phase_param.isSet()) this->set(phase_param.get());
	return *this;
}
vemt::type::PhaseParam & vemt::type::PhaseParam::operator=(const Phase phase) { this->set(phase); return *this; }
int vemt::type::PhaseParam::getAsInt(void) const { return static_cast<int>(this->get()); }
void vemt::type::PhaseParam::setAsInt(int int_value) { this->set(static_cast<Phase>(int_value)); }
std::wstring vemt::type::PhaseParam::toDisplayWstring() const noexcept {
	switch (this->get()) {
	case Phase::kPreEntry: return L"仮エントリー";
	case Phase::kEntry: return L"エントリー";
	case Phase::kSubmit: return L"入稿中";
	case Phase::kAccepted: return L"入稿完了";
	}
	wAbort("INVALID VALUE.");
	return std::wstring();
}

const std::string vemt::type::PhaseParam::toString() const {
	switch (this->get()) {
	case Phase::kPreEntry: return "pre-entry";
	case Phase::kEntry: return "entry";
	case Phase::kSubmit: return "submit";
	case Phase::kAccepted: return "accepted";
	}
	wAbort("INVALID VALUE.");
	return std::string();
}

bool vemt::type::PhaseParam::isAcceptable(const Phase & value) const {
	return true;
}

vemt::type::PhaseParam::operator int(void) const noexcept { return this->getAsInt(); }

vemt::type::Phase vemt::type::PhaseParam::parseFromString(const std::string & str) {
	if (str == "pre-entry")		return Phase::kPreEntry;
	else if (str == "entry")	return Phase::kEntry;
	else if (str == "submit")	return Phase::kSubmit;
	else if (str == "accepted") return Phase::kAccepted;
	throw ParseException("Failed to parse " + str + " as Phase.");
	return Phase();
}
