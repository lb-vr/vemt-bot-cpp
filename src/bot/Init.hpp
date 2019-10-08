#ifndef VEMT_BOT_INIT_HPP
#define VEMT_BOT_INIT_HPP

#include "bot/Process.hpp"

namespace vemt {
namespace bot {

class InitProcess : public OnMessageProcess {
public:
	InitProcess() noexcept;
	~InitProcess() noexcept;

	virtual std::unique_ptr<OnMessageProcess> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) override;

private:
};

}
}

#endif