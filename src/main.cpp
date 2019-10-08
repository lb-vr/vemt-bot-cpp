#include "bot/Client.hpp"

int main(int argc, char * argv[]) {

	vemt::bot::Client client = vemt::bot::Client::loadTokenFromFile("config/discord_token.txt");
	if (client.getToken().empty()) return -1;

	client.run();

	return 0;
}
