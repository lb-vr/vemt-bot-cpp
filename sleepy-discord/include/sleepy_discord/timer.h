#pragma once

#include <functional>

namespace dscd {

/// @brief Timer class.
struct Timer {
public:

	/// @brief Callback function type.
	typedef std::function<void()> StopTimerFunction;

	/// @brief Constructor
	///
	/// @deprecated does we need????
	Timer();

	/// @brief Constructor with callback function.
	Timer(StopTimerFunction stop_timer);

	/// @brief Call registed callback function.
	void stop() const;

	/// @brief is registed callback function?
	bool isValid() const;

private:
	StopTimerFunction impl_stop_;
};

}