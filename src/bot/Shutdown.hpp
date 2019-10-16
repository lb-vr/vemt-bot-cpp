#ifndef VEMT_BOT_SHUTDOWN_HPP
#define VEMT_BOT_SHUTDOWN_HPP

#include "OnMessageProcess.hpp"

namespace vemt {
namespace bot {

class ShutdownProcess : public OnMessageProcess {
public:
	ShutdownProcess() noexcept;
	~ShutdownProcess() noexcept;

	virtual std::unique_ptr<EventProcessBase> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void authenticate(Client & client, SleepyDiscord::Message & message) const override;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) override;

private:
};

}
}

#endif