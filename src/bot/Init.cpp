#include "Init.hpp"
#include "Client.hpp"

vemt::bot::InitProcess::InitProcess() noexcept
{}

vemt::bot::InitProcess::~InitProcess() noexcept
{}

std::unique_ptr<vemt::bot::OnMessageProcess> vemt::bot::InitProcess::create(void)
{ return std::make_unique<vemt::bot::InitProcess>(); }

std::string vemt::bot::InitProcess::getCommandStr(void) const
{ return "+init"; }

void vemt::bot::InitProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) {
	if (!message.author.bot) {
		client.sendMessage(message.channelID, message.author.username + "は" + message.content);
	}
}
