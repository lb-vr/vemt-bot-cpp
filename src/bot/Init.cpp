#include "Init.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"
#include "Settings.hpp"

namespace {
const std::string kBotCategoryName = "bot";
const std::string kContactCategoryName = "contact";

const std::string kBotControlChannelName = "bot-control";
const std::string kEntryChannelName = "entry";
const std::string kStatusChannelName = "status";
const std::string kQueryChannelName = "query";

const std::string kBotAdminRoleName = "BOT-Admin";
const std::string kExhibitorRoleName = "Exhibitor";
const std::string kManagerRoleName = "Manager";
}

vemt::bot::InitProcess::InitProcess() noexcept {}
vemt::bot::InitProcess::~InitProcess() noexcept {}

std::unique_ptr<vemt::bot::EventProcessBase> vemt::bot::InitProcess::create(void)
{ return std::make_unique<vemt::bot::InitProcess>(); }

std::string vemt::bot::InitProcess::getCommandStr(void) const
{ return "+init"; }

void vemt::bot::InitProcess::authenticate(Client & client, dscd::Message & message) const {
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"エントリーはサーバーのチャンネルでのみ受け付けています。");
	if (!this->isServerOwner(client, message))
		throw AuthenticationFailed(L"このコマンドを実行する権限がありません。");
}

void vemt::bot::InitProcess::run(Client & client, dscd::Message & message, const std::vector<std::string> & args) {
	logging::info << "Start to initialize server. serverID = " << message.serverID.string() << std::endl;
	client.sendMessageW(message.channelID, L"サーバーの初期化を開始します。初期化中は設定を変更しないでください。");

	logging::debug << " - Check if the server has already initialized." << std::endl;
	{
		logging::debug << " -- Checking channels." << std::endl;
		auto channels = client.getServerChannels(message.serverID).vector();
		logging::debug << " -- " << channels.size() << " channel(s) exist." << std::endl;
		for (const auto & ch : channels) {
			logging::debug << " --- Channel (" << ch.ID.string() << ") " << ch.name << std::endl;
			if (ch.name == kBotCategoryName ||
				ch.name == kContactCategoryName ||
				ch.name == kBotControlChannelName ||
				ch.name == kEntryChannelName ||
				ch.name == kStatusChannelName ||
				ch.name == kQueryChannelName) {
				logging::warn << "Abort initializing server. A Channel reserved name has already exists. Channel = " << ch.name << std::endl;
				throw ProcessException(L"既に予約されているカテゴリー・チャンネルが存在しています。");
			}
		}
		logging::debug << " -- Checked channels." << std::endl;

		logging::debug << " -- Checking roles." << std::endl;
		auto roles = client.getRoles(message.serverID).vector();
		logging::debug << " -- " << roles.size() << " role(s) exist." << std::endl;
		for (const auto & role : roles) {
			logging::debug << " --- Role (" << role.ID.number() << ") " << role.name << std::endl;
			if (role.name == kExhibitorRoleName ||
				role.name == kBotAdminRoleName ||
				role.name == kManagerRoleName) {
				logging::warn << "Abort initializing server. A Role reserved name has already exists. Role = " << role.name << std::endl;
				throw ProcessException(L"既に予約されているロールが存在しています。");
			}
		}
		logging::debug << " -- Checked roles." << std::endl;
	}
	logging::info << " - This server is not initialized." << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// DBを作る（デバッグ用）
	logging::debug << " - Creating DB (FOR DEVELOP ONLY)" << std::endl;
	const auto database_filename = this->getDatabaseFilepath(message);
	logging::debug << " -- Database filename = " << database_filename << std::endl;
	logging::debug << " -- (Require \"../src/scheme.sql\")" << std::endl;
	int ret = std::system(std::string("sqlite3 " + database_filename + " < ../src/scheme.sql").c_str());
	if (ret != 0) {
		logging::warn << "Failed to initialize sqlite database. sqlite3.exe returned " << ret << std::endl;
		throw ProcessException(L"Databaseの初期化に失敗しました。");
	}
	logging::info << " - sqlite3 returned 0." << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// everyone権限を取得
	logging::debug << " - Get everyone role ID." << std::endl;
	auto everyone = client.getRoleFromName(message.serverID, "@everyone");
	logging::info << " - @everyone roleID = " << everyone.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 自分の権限を取得
	logging::debug << " - Get vemt-bot role ID." << std::endl;
	auto vemt_bot_role = client.getRoleFromName(message.serverID, "vemt-bot");
	logging::info << " - vemt-bot roleID = " << vemt_bot_role.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 名前を変更
	logging::debug << " - Changing server nickname to VEMT." << std::endl;
	if (!client.editNickname(message.serverID, "VEMT")) {
		logging::warn << "Failed to change nickname to VEMT." << std::endl;
		throw ProcessException(L"ニックネームをVEMTに変更できませんでした。");
	}
	logging::info << " - Changed name to VEMT." << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// BOT管理者権限を作成
	logging::debug << " - Creating BOT-Admin role." << std::endl;
	auto bot_admin = client.createRole(message.serverID, kBotAdminRoleName, dscd::NONE, 0xff0000u, true, true).cast();
	logging::info << " - Created " << kBotAdminRoleName << " role. roleID = " << bot_admin.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 出展者権限を作成
	logging::debug << " - Creating roles." << std::endl;
	auto exhibitor_role = client.createRole(message.serverID, kExhibitorRoleName, dscd::NONE, 0x00ffffu, true, true).cast();
	logging::info << " - Created " << kExhibitorRoleName << " role. roleID = " << exhibitor_role.ID.string() << std::endl;
	auto manager_role = client.createRole(message.serverID, kManagerRoleName, dscd::NONE, 0x00cc00u, true, true).cast();
	logging::info << " - Created " << kManagerRoleName << " role. roleID = " << manager_role.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// カテゴリを作成
	logging::debug << " - Creating categories." << std::endl;
	auto bot_category = client.createCategory(message.serverID, kBotCategoryName).cast();
	logging::info << " - Created " << kBotCategoryName << " category. categoryID = " << bot_category.ID.string() << std::endl;
	auto contact_category = client.createCategory(message.serverID, kContactCategoryName).cast();
	logging::debug << " -- Editing " << kContactCategoryName << " category\'s permission." << std::endl;
	if (!client.editChannelPermissions(contact_category, dscd::Snowflake<dscd::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role")) {
		logging::warn << "Failed to edit permission of " << kContactCategoryName << " category." << std::endl;
		throw ProcessException(L"コンタクト用カテゴリの権限操作に失敗しました。");
	}
	logging::info << " - Created " << kContactCategoryName << " category. categoryID = " << contact_category.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Bot Control チャンネルを作成
	logging::debug << " - Creating " << kBotControlChannelName << " channel." << std::endl;

	auto bot_control_ch = client.createTextChannel(message.serverID, kBotControlChannelName, bot_category.ID).cast();
	logging::debug << " -- Created " << kBotControlChannelName << " channel. channelID = " << bot_control_ch.ID.number() << std::endl;
 
	client.editChannelTopic(bot_control_ch, u8"BOT操作専用チャンネルです。BOT-Adminのみが読み書きできます。`+config help`でヘルプを表示します。");
	logging::debug << " -- Edited channel topic." << std::endl;

	client.editChannelPermissions(bot_control_ch, dscd::Snowflake<dscd::Overwrite>(bot_admin.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " -- Edited channel permission for " << bot_admin.name << std::endl;
	
	client.editChannelPermissions(bot_control_ch, dscd::Snowflake<dscd::Overwrite>(vemt_bot_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
	logging::debug << " -- Edited channel permission for " << vemt_bot_role.name << std::endl;

	client.editChannelPermissions(bot_control_ch, dscd::Snowflake<dscd::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role");
	logging::debug << " -- Edited channel permission for " << everyone.name << std::endl;

	logging::info << " - Created " << kBotControlChannelName << " channel. channelID = " << bot_control_ch.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Entry チャンネルを作成
	logging::debug << " - Creating " << kEntryChannelName << " channel." << std::endl;
	auto entry_ch = client.createTextChannel(message.serverID, "entry", bot_category.ID).cast();
	logging::debug << " -- Created " << kEntryChannelName << " channel." << std::endl;

	client.editChannelTopic(entry_ch, u8"出展エントリー用のチャンネルです。エントリー期間中のみ書き込みでき、`+entry`コマンドでエントリーできます。");
	logging::debug << " -- Edited channel topic." << std::endl;

	client.editChannelPermissions(entry_ch, dscd::Snowflake<dscd::Overwrite>(vemt_bot_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
	logging::debug << " -- Edited channel permission for " << vemt_bot_role.name << std::endl;

	client.editChannelPermissions(entry_ch, dscd::Snowflake<dscd::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " -- Edited channel permission for " << everyone.name << std::endl;
	
	logging::info << " - Created " << kEntryChannelName << " channel. channelID = " << entry_ch.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Query チャンネルを作成
	logging::debug << " - Creating " << kQueryChannelName << " channel." << std::endl;
	auto query_ch = client.createTextChannel(message.serverID, kQueryChannelName, bot_category.ID).cast();
	logging::debug << " -- Created " << kQueryChannelName << " channel." << std::endl;

	client.editChannelTopic(query_ch, u8"BOTへ各種情報の問い合わせ用のチャンネルです。Manager権限の方が利用できます。`+query help`でヘルプを表示します。");
	logging::debug << " -- Edited channel topic." << std::endl;

	client.editChannelPermissions(query_ch, dscd::Snowflake<dscd::Overwrite>(vemt_bot_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
	logging::debug << " -- Edited channel permission for " << vemt_bot_role.name << std::endl;

	client.editChannelPermissions(query_ch, dscd::Snowflake<dscd::Overwrite>(manager_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
	logging::debug << " -- Edited channel permission for " << manager_role.name << std::endl;

	client.editChannelPermissions(query_ch, dscd::Snowflake<dscd::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role");
	logging::debug << " -- Edited channel permission for " << everyone.name << std::endl;

	logging::info << " - Created " << kQueryChannelName << " channel. channelID = " << query_ch.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Status チャンネルを作成
	logging::debug << " - Creating " << kStatusChannelName << " channel." << std::endl;
	auto status_ch = client.createTextChannel(message.serverID, kStatusChannelName, bot_category.ID).cast();
	logging::debug << " -- Created " << kStatusChannelName << " channel." << std::endl;

	client.editChannelTopic(status_ch, u8"BOTステータス確認用のチャンネルです。ミュート推奨です。");
	logging::debug << " -- Edited channel topic." << std::endl;

	client.editChannelPermissions(status_ch, dscd::Snowflake<dscd::Overwrite>(vemt_bot_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
	logging::debug << " -- Edited channel permission for " << vemt_bot_role.name << std::endl;

	client.editChannelPermissions(status_ch, dscd::Snowflake<dscd::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " -- Edited channel permission for " << everyone.name << std::endl;

	logging::info << " - Created " << kStatusChannelName << " channel. channelID = " << status_ch.ID.string() << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// セッティングに格納
	logging::debug << " - Saving settings." << std::endl;
	Settings(
		message.serverID.number(),
		bot_category.ID.number(),
		contact_category.ID.number(),
		bot_control_ch.ID.number(),
		entry_ch.ID.number(),
		status_ch.ID.number(),
		query_ch.ID.number(),
		bot_admin.ID.number(),
		exhibitor_role.ID.number(),
		manager_role.ID.number(),
		vemt_bot_role.ID.number(),
		everyone.ID.number()
	).save();
	logging::info << " - Saved settings." << std::endl;

	// 完了通知
	client.sendSuccessMessage(message.channelID, L"サーバーの初期化が完了しました。");
	logging::info << "Finished initializing the server. serverID = " << message.serverID.string() << std::endl;

}
