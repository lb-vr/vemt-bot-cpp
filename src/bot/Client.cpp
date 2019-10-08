/*If you are using uWebSockets,
  remember to change this to uwebsockets_websocket.h*/

#include "Client.hpp"
#include "util/string_util.hpp"
#include "Logger.hpp"
#include "Process.hpp"
#include <fstream>

vemt::bot::Client::Client(const std::string token)
	: SleepyDiscord::DiscordClient(token, 4), token_(token)
{}

vemt::bot::Client::Client(const Client & cp)
	: SleepyDiscord::DiscordClient(cp.getToken(), 4), token_(cp.getToken())
{}

const std::string & vemt::bot::Client::getToken(void) const	{
	return this->token_;
}

void vemt::bot::Client::onMessage(SleepyDiscord::Message message) {
	const auto args = strsplit(message.content, ' ');
	if (!args.empty()) {
		auto instance = OnMessageProcess::getClass(args[0]);
		if (instance) {
			try { instance->run(*this, message, args); }
			catch (SleepyDiscord::ErrorCode e) {
				logging::error << "Get " << e << " error from Discord. " << std::endl;
			}
		}
	}
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
