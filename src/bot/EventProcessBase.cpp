#include "EventProcessBase.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"

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
