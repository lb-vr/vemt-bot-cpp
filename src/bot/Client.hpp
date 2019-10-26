#ifndef VEMT_BOT_CLIENT_HPP
#define VEMT_BOT_CLIENT_HPP

#include "sleepy_discord/websocketpp_websocket.h"
#include <string>

namespace vemt {
namespace bot {

class Client : public dscd::DiscordClient {
public:
	Client(const std::string token);
	Client(const Client & cp);
	~Client(void) {}

	const std::string & getToken(void) const;

	virtual void onMessage(dscd::Message message) override;
	virtual void onResponse(dscd::Response responce) override;

	dscd::Role getRoleFromName(const dscd::Snowflake<dscd::Server> & serverID, const std::string & name);

	void sendSuccessMessage(const dscd::Snowflake<dscd::Channel> channelID, const std::wstring & message_str);
	void sendFailedMessage(const dscd::Snowflake<dscd::Channel> channelID, const std::wstring & message_str);
	dscd::ObjectResponse<dscd::Message> sendMessageW(dscd::Snowflake<dscd::Channel> channelID, std::wstring message, bool tts = false);
	dscd::ObjectResponse<dscd::Message> sendMentionW(dscd::Snowflake<dscd::Channel> channelID, dscd::User user, std::wstring message, bool tts = false);

	static Client loadTokenFromFile(const std::string & token_filepath);	

private:
	std::string token_;
};

}
}

#endif