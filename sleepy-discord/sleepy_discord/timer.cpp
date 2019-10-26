#include "timer.h"

dscd::Timer::Timer() {}

dscd::Timer::Timer(StopTimerFunction stop_timer) : impl_stop_(stop_timer) {}

void dscd::Timer::stop() const { this->impl_stop_(); }

bool dscd::Timer::isValid() const { return this->impl_stop_ != nullptr; }