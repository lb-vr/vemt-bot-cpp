﻿#ifndef VEMT_BOT_CLIENT_HPP
#define VEMT_BOT_CLIENT_HPP

#include "sleepy_discord/websocketpp_websocket.h"
#include <string>

namespace sd = SleepyDiscord;

namespace vemt {
namespace bot {

class Client : public SleepyDiscord::DiscordClient {
public:
	Client(const std::string token);
	Client(const Client & cp);
	~Client(void) {}

	const std::string & getToken(void) const;

	void onMessage(SleepyDiscord::Message message) override;

	sd::Role getRoleFromName(const sd::Snowflake<sd::Server> & serverID, const std::string & name);

	void sendSuccessMessage(const sd::Snowflake<sd::Channel> channelID, const std::string & message_str);
	void sendFailedMessage(const sd::Snowflake<sd::Channel> channelID, const std::string & message_str);

	static Client loadTokenFromFile(const std::string & token_filepath);	

private:
	std::string token_;
};

}
}

#endif