/*If you are using uWebSockets,
  remember to change this to uwebsockets_websocket.h*/

#include "Client.hpp"
#include "util/string_util.hpp"
#include "Logger.hpp"
#include <fstream>
/*
class MyClientClass : public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;
	void onMessage(SleepyDiscord::Message message) override {
		if (message.startsWith("whcg hello"))
			sendMessage(message.channelID, "Hello " + message.author.username);
	}
};

int main() {
	MyClientClass client("NjI3Nzc4Nzk2ODI0ODIxNzYx.XZSFlg.acN9BZTPLJffQKk_m1R2pljl5ak", 2);
	client.run();
}
*/
vemt::bot::Client::Client(const std::string token)
	: SleepyDiscord::DiscordClient(token, 4), token_(token)
{}

vemt::bot::Client::Client(const Client & cp)
	: SleepyDiscord::DiscordClient(cp.getToken(), 4), token_(cp.getToken())
{}

const std::string & vemt::bot::Client::getToken(void) const	{
	return this->token_;
}



void vemt::bot::Client::onMessage(SleepyDiscord::Message message)
{
	if (message.startsWith("whcg hello"))
		sendMessage(message.channelID, "Hello " + message.author.username);
}

vemt::bot::Client vemt::bot::Client::loadTokenFromFile(const std::string & token_filepath) {
	std::ifstream ifst(token_filepath);
	if (!ifst) {
		logging::error << "Token file is not found. PATH = " << token_filepath << std::endl;
		return Client("");
	}
	
	std::string token;
	if (!std::getline(ifst, token)) {
		logging::error << "Invalid token file. PATH = " << token_filepath << std::endl;
		return Client("");
	}

	token = strtrm(token);

	return Client(token);
}
