#include "Shutdown.hpp"
#include "Client.hpp"

vemt::bot::ShutdownProcess::ShutdownProcess() noexcept {}
vemt::bot::ShutdownProcess::~ShutdownProcess() noexcept {}

std::unique_ptr<vemt::bot::OnMessageProcess> vemt::bot::ShutdownProcess::create(void)
{ return std::make_unique<vemt::bot::ShutdownProcess>(); }

std::string vemt::bot::ShutdownProcess::getCommandStr(void) const
{ return "+shutdown"; }

void vemt::bot::ShutdownProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	if (this->isServerOwner(client, message)) {
		client.sendMessage(message.channelID, "Shutdown. See you again.");
		client.quit();
	}
}
