#include "EventProcessBase.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"
#include "util/string_util.hpp"

vemt::bot::EventProcessBase::EventProcessBase() noexcept {}
vemt::bot::EventProcessBase::~EventProcessBase() noexcept {}

bool vemt::bot::EventProcessBase::isServerOwner(Client & client, const SleepyDiscord::Message & message){
	return (message.author.ID == client.getServer(message.serverID).cast().ownerID);
}

bool vemt::bot::EventProcessBase::isBotAdmin(Client & client, const SleepyDiscord::Message & message){
	auto user_roles = client.getMember(message.serverID, message.author.ID).cast().roles;
	auto server_roles = client.getRoles(message.serverID).vector();
	for (const auto & r : server_roles) {
		if (r.name == "BOT-Admin") {
			for (const auto & p : user_roles) {
				if (p == r.ID) return true;
			}
			break;
		}
	}
	return false;
}

bool vemt::bot::EventProcessBase::isServer(Client & client, const SleepyDiscord::Message & message) {
	return (client.getChannel(message.channelID).cast().type == sd::Channel::ChannelType::SERVER_TEXT);
}

bool vemt::bot::EventProcessBase::isRole(Client & client, const SleepyDiscord::Message & message, const int64_t & roleID)
{
	auto user_roles = client.getMember(message.serverID, message.author.ID).cast().roles;
	for (const auto & p : user_roles) {
		if (p.number() == roleID) return true;
	}
	return false;
}

std::string vemt::bot::EventProcessBase::getDatabaseFilepath(const SleepyDiscord::Message & message) {
	return message.serverID.string() + ".db";
}

vemt::bot::EventProcessBase::AuthenticationFailed::AuthenticationFailed(const std::wstring & error_message) noexcept
	: std::runtime_error("Authentication Failed."), error_message_(error_message) {}

const std::wstring & vemt::bot::EventProcessBase::AuthenticationFailed::getErrorMessage() const { return this->error_message_; }

vemt::bot::ProcessException::ProcessException(const std::wstring & log) : std::runtime_error(util::narrow(log)) {
	logging::warn << "Process failed. Message = " << util::narrow(log) << std::endl;
}
