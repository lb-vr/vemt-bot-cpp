#include "Shutdown.hpp"
#include "Client.hpp"

vemt::bot::ShutdownProcess::ShutdownProcess() noexcept {}
vemt::bot::ShutdownProcess::~ShutdownProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::ShutdownProcess::create(void)
{ return std::make_unique<vemt::bot::ShutdownProcess>(); }

std::string vemt::bot::ShutdownProcess::getCommandStr(void) const
{ return "+shutdown"; }

void vemt::bot::ShutdownProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
	if (!this->isServerOwner(client, message))
		throw AuthenticationFailed(L"このコマンドを実行する権限がありません。");
}

void vemt::bot::ShutdownProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	client.sendMessageW(message.channelID, L"さようなら、また会う日まで。");
	client.quit();
}
