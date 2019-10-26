#include "Config.hpp"
#include "Client.hpp"
#include "Settings.hpp"
#include "util/Logger.hpp"
#include "util/json11.hpp"
#include "util/string_util.hpp"
#include "Question.hpp"
#include "db/QuestionItemsTable.hpp"

vemt::bot::ConfigProcess::ConfigProcess() noexcept : OnMessageProcess() {}

vemt::bot::ConfigProcess::~ConfigProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::ConfigProcess::create(void) {
	return std::make_unique<ConfigProcess>();
}

std::string vemt::bot::ConfigProcess::getCommandStr(void) const {
	return "+config";
}

void vemt::bot::ConfigProcess::authenticate(Client & client, dscd::Message & message) const {
	auto serverID = message.serverID.number();
	auto bot_admin_role = Settings::getSettings(serverID).getBotAdminRole();
	auto bot_control_ch = Settings::getSettings(serverID).getBotControlChannel();
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"+configコマンドはサーバーのbot-controlチャンネルでのみ受け付けています。");
	if (message.channelID.number() != bot_control_ch)
		throw AuthenticationFailed(L"+configコマンドはbot-controlチャンネルでのみ受け付けています。");
	if (!this->isBotAdmin(client, message))
		throw AuthenticationFailed(L"+configコマンドを実行する権限がありません。");
}

void vemt::bot::ConfigProcess::run(Client & client, dscd::Message & message, const std::vector<std::string> & args) {
	logging::debug << "args length = " << args.size() << std::endl;
	if (args.size() == 3) {
		logging::debug << "args [0]" << args.at(0) << " [1]" << args.at(1) << " [2]" << args.at(2) << std::endl;
		logging::debug << "embeds size : " << message.embeds.size() << std::endl;
		
		if (args.at(1) == "question" && args.at(2) == "upload" && message.attachments.size() == 1) {
			auto attachments = message.attachments.at(0);
			this->question_upload(client, message, attachments);
		}
		else if (args.at(1) == "question" && args.at(2) == "add") {
			this->question_add(client, message, args.at(2));
		}
		else {
			client.sendFailedMessage(message.channelID, L"構文エラーです。`+config help`でヘルプを表示します。");
		}
	}
}

void vemt::bot::ConfigProcess::question_upload(Client & client, dscd::Message & message, const dscd::Attachment & obj) {
	// Get json from URL
	logging::debug << " - Creating session for download json file." << std::endl;
	logging::debug << " - URL = " << obj.url << std::endl;

	sd::Session session;
	session.setUrl(obj.url);
	session.setHeader({
		sd::HeaderPair("Connection", "close"),
		sd::HeaderPair("User-Agent", "vemt-bot/0.0.1 (https://github.com/lb-vr/vemt-bot-cpp, develop)"),
		sd::HeaderPair("Accept","*/*") });
	auto response = session.Get();
	if (response.error()) {
		logging::warn << "Failed to get json file. response error. error code " << response.statusCode << ", detail = " << response.text << std::endl;
		client.sendFailedMessage(message.channelID, L"jsonファイルの取得に失敗しました。");
		return;
	}
	logging::info << " - Created and fetched json file. Status code = " << response.statusCode << std::endl;

	// Create Question from Json
	logging::debug << " - Parsing from downloaded json file." << std::endl;
	auto question = Question::loadFromJson(response.text);
	logging::info << " - Parsed json file and created Question instance." << std::endl;

	// Add to Database.
	logging::debug << " - Registing questions to database." << std::endl;
	auto qitems = question.getQuestionItem();
	std::vector<db::QuestionItemModel> qitems_m;
	for (const auto q : qitems) qitems_m.emplace_back(q);
	db::QuestionItemsTable question_items_table(this->getDatabaseFilepath(message));
	question_items_table.replaceAll(qitems_m);
	logging::info << " - Registed questions to database." << std::endl;

	// preview
	logging::debug << " - Sending preview messages." << std::endl;
	client.sendSuccessMessage(message.channelID, L"質問の登録・更新が完了しました。プレビューを表示します。");
	auto renewed_question = Question::loadFromDatabase(this->getDatabaseFilepath(message));
	client.sendMessageW(message.channelID, renewed_question.generateQuestionHeaderMessage());
	client.sendMessageW(message.channelID, renewed_question.generateQuestionItemsMessage());
	logging::info << " - Sent preview message." << std::endl;
	logging::info << "Finished config question." << std::endl;
}

#include "db/EntriesTable.hpp"
void vemt::bot::ConfigProcess::question_add(Client & client, dscd::Message & message, const std::string & param) {
	db::EntriesTable(this->getDatabaseFilepath(message)).getById(1);
}
