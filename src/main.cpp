#define _CRT_SECURE_NO_WARNINGS
#include "bot/Client.hpp"

// Process関連
#include "bot/Init.hpp"
#include "bot/Reset.hpp"
#include "bot/Shutdown.hpp"
#include "bot/Entry.hpp"
#include "bot/Config.hpp"
#include "util/Logger.hpp"

int main(int argc, char * argv[]) {
	logging::Logger::setOutputAllLevelEnabled();
	logging::Logger::setOutputAllDistinationEnabled();
	//logging::Logger::setRedirectionCout(logging::Logger::kInfo);
	//logging::Logger::setRedirectionCerr(logging::Logger::kDebug);
	logging::debug << "vemt-bot-cpp version 0.0.1" << std::endl;

	// Processの登録
	vemt::bot::OnMessageProcess::addClass(std::make_unique<vemt::bot::InitProcess>());
	vemt::bot::OnMessageProcess::addClass(std::make_unique<vemt::bot::ResetProcess>());
	vemt::bot::OnMessageProcess::addClass(std::make_unique<vemt::bot::ShutdownProcess>());
	vemt::bot::OnMessageProcess::addClass(std::make_unique<vemt::bot::EntryProcess>());
	vemt::bot::OnMessageProcess::addClass(std::make_unique<vemt::bot::ConfigProcess>());

	vemt::bot::Client client = vemt::bot::Client::loadTokenFromFile("config/discord_token.txt");
	if (client.getToken().empty()) return -1;

	client.run();

	return 0;
}
