#include "Answer.hpp"
#include "util/Logger.hpp"
#include "Client.hpp"
#include "bot/QuestionItem.hpp"
#include "db/QuestionItemsTable.hpp"
#include "db/DatabaseException.hpp"

vemt::bot::AnswerProcess::AnswerProcess() noexcept : EventProcessBase(){}

vemt::bot::AnswerProcess::~AnswerProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::AnswerProcess::create(void) {
	return std::unique_ptr<AnswerProcess>();
}

std::string vemt::bot::AnswerProcess::getCommandStr(void) const {
	return "+answer";
}

void vemt::bot::AnswerProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
}

void vemt::bot::AnswerProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	// This command is only for direct message.
	// もし公開サーバーで回答された場合は、回答として採用するが、その後すぐメッセージを削除する

	// argument parser and validating
	if (args.size() <= 2) {
		logging::warn << "User " << message.author.username << "#" << message.author.discriminator << " Answer rejected. Invalid arguments." << std::endl;
	}
	try {
		int question_id = std::stoi(args.at(1));

		// Question IDが問題としてあるか？
		QuestionItem question_item(db::QuestionItemsTable("test.sqlite3").getById(question_id));

	
	}
	catch(std::invalid_argument){
		logging::warn << "User " << message.author.username << "#" << message.author.discriminator << " Answer rejected. Invalid ID." << std::endl;
		client.sendFailedMessage(message.channelID, L"問題IDが不正です。");
	}

	// user
	//const sd::User & user = message.author;

	/*if (this->isServer(client, message)) {
		logging::warn << "User " << message.author.username << "#" << message.author.discriminator << " Answer rejected. Not on DirectMessage." << std::endl;
		return;
	}*/

}
