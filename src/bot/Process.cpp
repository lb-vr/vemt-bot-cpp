#include "Process.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"

std::unordered_map<std::string, std::unique_ptr<vemt::bot::OnMessageProcess>> vemt::bot::OnMessageProcess::class_instances_;

vemt::bot::OnMessageProcess::OnMessageProcess() noexcept {}
vemt::bot::OnMessageProcess::~OnMessageProcess() noexcept {}

bool vemt::bot::OnMessageProcess::addClass(std::unique_ptr<OnMessageProcess>&& instance) {
	auto cmd = instance->getCommandStr();
	if (OnMessageProcess::class_instances_.count(cmd)) {
		logging::error << "Failed to addClass, already added. Command = " << cmd << std::endl;
		return false;
	}
	OnMessageProcess::class_instances_[cmd] = std::move(instance);
	return true;
}

std::unique_ptr<vemt::bot::OnMessageProcess> vemt::bot::OnMessageProcess::getClass(const std::string & cmd) {
	if (OnMessageProcess::class_instances_.count(cmd)) {
		return OnMessageProcess::class_instances_.at(cmd)->create();
	}
	return std::unique_ptr<OnMessageProcess>();	// nullptr
}

bool vemt::bot::OnMessageProcess::isServerOwner(Client & client, const SleepyDiscord::Message & message){
	return (message.author.ID == client.getServer(message.serverID).cast().ownerID);
}

bool vemt::bot::OnMessageProcess::isBotAdmin(Client & client, const SleepyDiscord::Message & message){
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

bool vemt::bot::OnMessageProcess::isServer(Client & client, const SleepyDiscord::Message & message) {
	return (client.getChannel(message.channelID).cast().type == sd::Channel::ChannelType::SERVER_TEXT);
}
