#ifndef VEMT_BOT_EVENTPROCESSBASE_HPP
#define VEMT_BOT_EVENTPROCESSBASE_HPP

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace SleepyDiscord {
class Message;
class User;
}

namespace vemt {
namespace bot {

class Client;

class ProcessException : std::runtime_error {
public:
	ProcessException(const std::wstring & log);
};

class EventProcessBase {
public:

	class AuthenticationFailed : public std::runtime_error{
	public:
		explicit AuthenticationFailed(const std::wstring & error_message) noexcept;
		const std::wstring & getErrorMessage() const;
	private:
		const std::wstring error_message_;
	};

	EventProcessBase() noexcept;
	virtual ~EventProcessBase() noexcept;

	virtual std::unique_ptr<EventProcessBase> create() = 0;
	virtual std::string getCommandStr(void) const = 0;
	virtual void authenticate(Client & client, SleepyDiscord::Message & message) const = 0;
	virtual void run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) = 0;

	static bool isServerOwner(Client & client, const SleepyDiscord::Message & message);
	static bool isBotAdmin(Client & client, const SleepyDiscord::Message & message);
	static bool isServer(Client & client, const SleepyDiscord::Message & message);
	static bool isRole(Client & client, const SleepyDiscord::Message & message, const int64_t & roleID);
	static std::string getDatabaseFilepath(const SleepyDiscord::Message & message);

private:
};

}
}

#endif