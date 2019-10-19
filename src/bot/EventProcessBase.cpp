#include "EventProcessBase.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"
#include "util/string_util.hpp"

vemt::bot::EventProcessBase::EventProcessBase() noexcept {}
vemt::bot::EventProcessBase::~EventProcessBase() noexcept {}

bool vemt::bot::EventProcessBase::isServerOwner(Client & client, const SleepyDiscord::Message & message){
	bool ret = (message.author.ID == client.getServer(message.serverID).cast().ownerID);
	logging::debug << "isServerOwner ? " << (ret ? "true": "false") << std::endl;
	return ret;
}

bool vemt::bot::EventProcessBase::isBotAdmin(Client & client, const SleepyDiscord::Message & message){
	auto user_roles = client.getMember(message.serverID, message.author.ID).cast().roles;
	auto server_roles = client.getRoles(message.serverID).vector();
	bool ret = false;
	for (const auto & r : server_roles) {
		if (r.name == "BOT-Admin") {
			for (const auto & p : user_roles) {
				if (p == r.ID) {
					ret = true;
					break;
				}
			}
			break;
		}
	}
	logging::debug << "isBotAdmin ? " << (ret ? "true" : "false") << std::endl;
	return ret;
}

bool vemt::bot::EventProcessBase::isServer(Client & client, const SleepyDiscord::Message & message) {
	auto ret = (client.getChannel(message.channelID).cast().type == sd::Channel::ChannelType::SERVER_TEXT);
	logging::debug << "isServer ? " << (ret ? "true" : "false") << std::endl;
	return ret;
}

bool vemt::bot::EventProcessBase::isRole(Client & client, const SleepyDiscord::Message & message, const int64_t & roleID) {
	auto user_roles = client.getMember(message.serverID, message.author.ID).cast().roles;
	bool ret = false;
	for (const auto & p : user_roles) {
		if (p.number() == roleID) {
			ret = true;
			break;
		}
	}
	logging::debug << "User " << message.author.username << " is Role[" << roleID << "] ? " << (ret ? "true" : "false") << std::endl;
	return ret;
}

std::string vemt::bot::EventProcessBase::getDatabaseFilepath(const SleepyDiscord::Message & message) {
	 auto name = message.serverID.string() + ".db";
	 logging::debug << "Requested database server filename = " << name << std::endl;
	 return name;
}

vemt::bot::EventProcessBase::AuthenticationFailed::AuthenticationFailed(const std::wstring & error_message) noexcept
	: std::runtime_error("Authentication Failed."), error_message_(error_message) {}

const std::wstring & vemt::bot::EventProcessBase::AuthenticationFailed::getErrorMessage() const { return this->error_message_; }

vemt::bot::ProcessException::ProcessException(const std::wstring & log) : std::runtime_error(util::narrow(log)), error_log_(log) {}

const std::wstring & vemt::bot::ProcessException::getErrorMessage(void) const
{ return this->error_log_; }
