#pragma once
#ifndef SLEEPY_DO_NOT_INCLUDE_STANDARD_ONERROR
virtual void onError(dscd::ErrorCode errorCode, const std::string errorMessage);
#endif

#ifndef SLEEPY_DO_NOT_INCLUDE_STANDARD_SLEEP
virtual void sleep(const unsigned int milliseconds);
#endif