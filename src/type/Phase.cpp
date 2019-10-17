#include "Phase.hpp"

const vemt::Phase vemt::Phase::kPreEntry = 1;
const vemt::Phase vemt::Phase::kEntry = 2;
const vemt::Phase vemt::Phase::kSubmit = 3;
const vemt::Phase vemt::Phase::kPublish = 100;

vemt::Phase::Phase(const int phase_int) noexcept : phase_int_(phase_int) {}
vemt::Phase::Phase(const Phase & phase) noexcept : phase_int_(phase.phase_int_) {}
vemt::Phase & vemt::Phase::operator=(const Phase & phase) noexcept { this->phase_int_ = phase.to_int(); return *this; }
vemt::Phase & vemt::Phase::operator=(const int phase_int) noexcept { this->phase_int_ = phase_int; return *this; }
vemt::Phase::~Phase() noexcept {}
const bool vemt::Phase::operator==(const Phase & phase) const noexcept { return this->to_int() == phase.to_int(); }
const bool vemt::Phase::operator!=(const Phase & phase) const noexcept { return this->to_int() != phase.to_int(); }
const bool vemt::Phase::operator>(const Phase & phase) const noexcept { return this->to_int() > phase.to_int(); }
const bool vemt::Phase::operator<(const Phase & phase) const noexcept { return this->to_int() < phase.to_int(); }
const bool vemt::Phase::operator==(const int phase) const noexcept { return this->to_int() == phase; }
const bool vemt::Phase::operator!=(const int phase) const noexcept { return this->to_int() != phase; }
const bool vemt::Phase::operator>(const int phase) const noexcept { return this->to_int() > phase; }
const bool vemt::Phase::operator<(const int phase) const noexcept { return this->to_int() < phase; }
int vemt::Phase::to_int(void) const noexcept { return this->phase_int_; }
std::wstring vemt::Phase::toDisplayWstring() const noexcept {
	return L"[TEMP]仮エントリー";
}
vemt::Phase::operator int(void) const noexcept { return this->to_int(); }

