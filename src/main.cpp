#include "bot/Client.hpp"

// ProcessŠÖ˜A
#include "bot/Init.hpp"

int main(int argc, char * argv[]) {

	// Process‚Ì“o˜^
	vemt::bot::OnMessageProcess::addClass(std::make_unique<vemt::bot::InitProcess>());

	vemt::bot::Client client = vemt::bot::Client::loadTokenFromFile("config/discord_token.txt");
	if (client.getToken().empty()) return -1;

	client.run();

	return 0;
}
