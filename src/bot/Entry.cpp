#include "Entry.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"
#include "util/string_util.hpp"
#include "Question.hpp"
#include "Settings.hpp"
#include "db/EntriesTable.hpp"
#include "db/DatabaseException.hpp"

vemt::bot::EntryProcess::EntryProcess() noexcept {}

vemt::bot::EntryProcess::~EntryProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::EntryProcess::create(void)
{ return std::make_unique<EntryProcess>(); }

std::string vemt::bot::EntryProcess::getCommandStr(void) const { return "+entry"; }

void vemt::bot::EntryProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
	const Settings & settings = Settings::getSettings(message.serverID.number());
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"エントリーはサーバーのentryチャンネルでのみ受け付けています。");
	if (settings.getEntryChannel() != message.channelID.number())
		throw AuthenticationFailed(L"エントリーはentryチャンネルでのみ受け付けています。");
	// TODO: 開催期間かどうか？
	
	// データベースにアクセスして、既に仮エントリー済みでないか調べる
	auto _author_id = message.author.ID.number();
	auto entried = db::EntriesTable(message.serverID.string() + ".db").getByDiscordUid(_author_id);
	if (!entried.empty()) throw AuthenticationFailed(L"既にエントリー済みです。");
}

void vemt::bot::EntryProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string>& args) {
	logging::info << "Start to EntryProcess. User = " << message.author.username << "#" << message.author.discriminator << std::endl;
	auto temporary_message = client.sendMessageW(message.channelID, L"エントリー受付の処理を開始しました。しばらくお待ちください。").cast();

	logging::debug << " - Loading settings. ServerID = " << message.serverID.number() << std::endl;
	const Settings & settings = Settings::getSettings(message.serverID.number());
	logging::debug << " - Finished loading settings." << std::endl;

	// 専用チャンネルを作成する
	logging::debug << " - Creating a text channel for direct messaging." << std::endl;
	auto dm_channel = client.createTextChannel(message.serverID, message.author.username + "_" + message.author.discriminator,
		sd::Snowflake<sd::Channel>(settings.getContactCategory())).cast();
	logging::debug << " -- Created a text channel named " << dm_channel.name << std::endl;

	bool _ok = true;
	_ok &= client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(settings.getVemtBotRole()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES | sd::Permission::MANAGE_ROLES, 0, "role");
	logging::debug << " -- Edited channel permission for vemt-bot(" << settings.getVemtBotRole() << ")." << std::endl;
	
	_ok &= client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(settings.getEveryoneRole()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES | sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " -- Edited channel permission for everyone(" << settings.getEveryoneRole() << ")." << std::endl;

	_ok &= client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(message.author.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, sd::Permission::MANAGE_ROLES, "member");
	logging::debug << " -- Edited channel permission for User " << message.author.username << " (" << message.author.ID.string() << ")." << std::endl;

	_ok &= client.editChannelPermissions(dm_channel, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(settings.getBotAdminRole()),
		sd::Permission::READ_MESSAGES, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " -- Edited channel permission for bot-admin(" << settings.getBotAdminRole() << ")." << std::endl;

	if (!_ok) {
		logging::warn << "Failed to edit permissions." << std::endl;
		logging::debug << "Try to delete direct message channel." << std::endl;
		client.deleteChannel(dm_channel.ID);
		logging::info << "Tried to delete " << dm_channel.name << " text channel." << std::endl;
		throw ProcessException(L"ダイレクトメッセージ用チャンネルの権限操作に失敗しました。");
	}
	logging::info << " - Created and setup a text channel. Channel = " << dm_channel.name << " (" << dm_channel.ID.string() << ")" << std::endl;

	// 質問をサーバーから取り寄せる
	logging::debug << " - Fetching questionary from database." << std::endl;
	auto questionary = Question::loadFromDatabase(this->getDatabaseFilepath(message));
	logging::info << " - Fetched questionary from database." << std::endl;

	// メッセージの送信
	logging::debug << " - Sending header message to DM text chat." << std::endl;
	client.sendMessageW(dm_channel, questionary.generateQuestionHeaderMessage());
	logging::info << " - Sent header message to DM text chat." << std::endl;

	logging::debug << " - Sending question item message to DM text chat." << std::endl;
	auto qitem_msg = client.sendMessageW(dm_channel, questionary.generateQuestionItemsMessage()).cast();
	logging::debug << " -- Sent question item message to DM text chat." << std::endl;
	if (!client.pinMessage2(dm_channel, qitem_msg)) {
		logging::warn << "Failed to pin question item message, but ignore and continue process." << std::endl;
	}
	else logging::info << " - Sent question item message to DM text chat and pinned it." << std::endl;

	// DB登録
	logging::debug << " - Registing entry to database." << std::endl;
	auto entrying_result = db::EntriesTable(this->getDatabaseFilepath(message)).insert(
		db::EntryModel(message.author.ID.number(), type::Phase::kPreEntry, dm_channel.ID.number(), qitem_msg.ID.number()));
	wAssertM(entrying_result.size() == 1, "Database returned illegal data.");

	logging::info << " - Accepted entry." << std::endl;
	logging::info << " -- Entry ID   = " << entrying_result.at(0).getId() << std::endl;
	logging::info << " -- Discord ID = " << message.author.ID.string() << std::endl;
	logging::info << " -- Username   = " << message.author.username << "#" << message.author.discriminator << std::endl;
	logging::info << " -- Created at = " << entrying_result.at(0).getCreatedAt().toString() << std::endl;

	logging::debug << " - Deleting temporary message. messageID = " << temporary_message.ID.string() << std::endl;
	client.deleteMessage2(temporary_message.channelID, temporary_message);
	logging::info << " - Deleted temporary message." << std::endl;

	// 仮エントリーを受け付けました、DMを確認してくださいとメッセージ	
	client.sendMentionW(message.channelID, message.author, L"仮エントリーを受け付けました。CONTACT 個別専用チャンネルを確認してください。");
	logging::info << "Accepted pre-entry." << std::endl;
}