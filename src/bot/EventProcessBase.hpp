#ifndef VEMT_BOT_EVENTPROCESSBASE_HPP
#define VEMT_BOT_EVENTPROCESSBASE_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace SleepyDiscord {
class Message;
}

namespace vemt {
namespace bot {

class Client;

class EventProcessBase {
public:
	EventProcessBase() noexcept;
	virtual ~EventProcessBase() noexcept;

	virtual std::unique_ptr<EventProcessBase> create() = 0;
	virtual std::string getCommandStr(void) const = 0;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) = 0;

	static bool isServerOwner(Client & client, const SleepyDiscord::Message & message);
	static bool isBotAdmin(Client & client, const SleepyDiscord::Message & message);
	static bool isServer(Client & client, const SleepyDiscord::Message & message);

private:
};

}
}

#endif