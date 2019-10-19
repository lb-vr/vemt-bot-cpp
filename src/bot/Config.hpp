#ifndef VEMT_BOT_CONFIG_HPP
#define VEMT_BOT_CONFIG_HPP

#include "OnMessageProcess.hpp"

namespace SleepyDiscord {
class Attachment;
}

namespace vemt {
namespace bot {

class ConfigProcess : public OnMessageProcess {
public:
	ConfigProcess() noexcept;
	~ConfigProcess() noexcept;

	virtual std::unique_ptr<EventProcessBase> create(void) override;
	virtual std::string getCommandStr(void) const override;
	virtual void authenticate(Client & client, SleepyDiscord::Message & message) const override;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) override;

private:
	void question_upload(Client & client, SleepyDiscord::Message & message, const SleepyDiscord::Attachment & fpath);
	void question_add(Client & client, SleepyDiscord::Message & message, const SleepyDiscord::Attachment & fpath);
};

}
}

#endif