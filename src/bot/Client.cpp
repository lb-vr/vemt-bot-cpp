/*If you are using uWebSockets,
  remember to change this to uwebsockets_websocket.h*/

#include "Client.hpp"
#include "util/string_util.hpp"
#include "util/Logger.hpp"
#include "OnMessageProcess.hpp"
#include "OnEditMessageProcess.hpp"
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
	logging::trace << "onMessage called. mID=" << message.ID.string() << std::endl;
	logging::trace << " - " << message.ID.string() << " : author=" << message.author.username << "#" << message.author.discriminator << "(" << message.author.ID.string() << ")" << std::endl;
	logging::trace << " - " << message.ID.string() << " : serverID=" << message.serverID.string() << std::endl;
	logging::trace << " - " << message.ID.string() << " : channelID=" << message.channelID.string() << std::endl;
	const auto args = util::strsplit(message.content, ' ');
	if (!args.empty()) {
		auto instance = OnMessageProcess::getClass(args[0]);
		if (instance) {
			try { 
				logging::info << " - " << message.ID.string() << " : Received command = " << message.content << " cmd = " << args[0] << std::endl;
				instance->authenticate(*this, message);
				instance->run(*this, message, args);				
			}
			catch (EventProcessBase::AuthenticationFailed e) {
				this->sendFailedMessage(message.channelID, e.getErrorMessage());
				logging::warn << "Authentication Failed. User=" << message.author.username << "#" << message.author.discriminator
					<< " Message=" << util::narrow(e.getErrorMessage()) << std::endl;
			}
			catch (SleepyDiscord::ErrorCode e) {
				logging::error << "Get " << e << " error from Discord. " << std::endl;
			}
		}
	}
}

void vemt::bot::Client::onResponse(SleepyDiscord::Response response) {
	if (response.error()) logging::warn << "Get Error " << response.statusCode << " Response. Message = " << response.text << std::endl;
	//logging::debug << "Get " << response.statusCode << " response. Message = " << response.text << std::endl;
}

sd::Role vemt::bot::Client::getRoleFromName(const sd::Snowflake<sd::Server> & serverID, const std::string & name) {
	auto roles = this->getRoles(serverID).vector();
	for (const auto & r : roles) {
		if (r.name == name) {
			return r;
		}
	}
	return sd::Role();
}

void vemt::bot::Client::sendSuccessMessage(const sd::Snowflake<sd::Channel> channelID, const std::wstring & message_str)
{ this->sendMessageW(channelID, L"**成功** " + message_str); }

void vemt::bot::Client::sendFailedMessage(const sd::Snowflake<sd::Channel> channelID, const std::wstring & message_str)
{ this->sendMessageW(channelID, L"**失敗** " + message_str); }

sd::ObjectResponse<sd::Message> vemt::bot::Client::sendMessageW(sd::Snowflake<sd::Channel> channelID, std::wstring message, bool tts) {
	return this->sendMessage(channelID, util::narrow(message), tts);
}

sd::ObjectResponse<sd::Message> vemt::bot::Client::sendMentionW(sd::Snowflake<sd::Channel> channelID, sd::User user, std::wstring message, bool tts) {
	return this->sendMention(channelID, user, util::narrow(message), tts);
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

	token = util::strtrm(token);

	return Client(token);
}
