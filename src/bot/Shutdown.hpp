#ifndef VEMT_BOT_SHUTDOWN_HPP
#define VEMT_BOT_SHUTDOWN_HPP

#include "bot/Process.hpp"

namespace vemt {
namespace bot {

class ShutdownProcess : public OnMessageProcess {
public:
	ShutdownProcess() noexcept;
	~ShutdownProcess() noexcept;

	virtual std::unique_ptr<OnMessageProcess> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) override;

private:
};

}
}

#endif