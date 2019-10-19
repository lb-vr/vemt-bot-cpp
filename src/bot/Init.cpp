#include "Init.hpp"
#include "Client.hpp"
#include "util/Logger.hpp"
#include "Settings.hpp"

namespace {
const std::string kBotCategoryName = "bot";
const std::string kBotControlChannelName = "bot-control";
const std::string kEntryChannelName = "entry";
const std::string kStatusChannelName = "status";
const std::string kContactCategoryName = "contact";
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

void vemt::bot::InitProcess::authenticate(Client & client, SleepyDiscord::Message & message) const {
	if (!this->isServer(client, message))
		throw AuthenticationFailed(L"エントリーはサーバーのチャンネルでのみ受け付けています。");
	if (!this->isServerOwner(client, message))
		throw AuthenticationFailed(L"このコマンドを実行する権限がありません。");
}

// #include <cstdlib>
void vemt::bot::InitProcess::run(Client & client, SleepyDiscord::Message & message, const std::vector<std::string> & args) {
	logging::info << "Start to initialize server. serverID = " << message.serverID.string() << std::endl;
	client.sendMessageW(message.channelID, L"サーバーの初期化を開始します。初期化中は設定を変更しないでください。");

	logging::debug << "Check if the server has already initialized." << std::endl;
	// TODO: 既に予約されているチャンネル名などがないかチェックを行う
	{
		auto channels = client.getServerChannels(message.serverID).vector();
		logging::debug << " - channels (" << channels.size() << ")" << std::endl;
		for (const auto & ch : channels) {
			logging::debug << " - : " << ch.name << " (" << ch.ID.string() << ")" << std::endl;
			if (ch.name == kBotCategoryName ||
				ch.name == kBotControlChannelName ||
				ch.name == kEntryChannelName ||
				ch.name == kStatusChannelName ||
				ch.name == kContactCategoryName) {
				client.sendFailedMessage(message.channelID, L"既に予約されているカテゴリー・チャンネルが存在しています。");
				logging::warn << "Stop initialize server. Already existing channel. Channel = " << ch.name << std::endl;
				return;
			}
		}
		auto roles = client.getRoles(message.serverID).vector();
		logging::debug << " - roles (" << roles.size() << ")" << std::endl;
		for (const auto & role : roles) {
			logging::debug << " - : " << role.name << " (" << role.ID.string() << ")" << std::endl;
			if (role.name == kExhibitorRoleName ||
				role.name == kBotAdminRoleName ||
				role.name == kManagerRoleName) {
				client.sendFailedMessage(message.channelID, L"既に予約されているロールが存在しています。");
				logging::warn << "Stop initialize server. Already existing role. Role = " << role.name << std::endl;
				return;
			}
		}
	}

	// DBを作る（デバッグ用）
	const auto database_filename = this->getDatabaseFilepath(message);
	logging::debug << " - Creating database. Database filename = " << database_filename << std::endl;
	logging::debug << " - (Require \"../src/scheme.sql\")" << std::endl;
	int ret = std::system(std::string("sqlite3 " + database_filename + " < ../src/scheme.sql").c_str());
	logging::debug << "sqlite3 returned " << ret << "." << std::endl;

	// everyone権限を取得
	auto everyone = client.getRoleFromName(message.serverID, "@everyone");
	logging::debug << " - @everyone role ID = " << everyone.ID.string() << ". serverID = " << message.serverID.string() << std::endl;

	// 自分の権限を取得
	auto vemt_bot_role = client.getRoleFromName(message.serverID, "vemt-bot");
	logging::debug << " - vemt-bot role ID = " << vemt_bot_role.ID.string() << ". serverID = " << message.serverID.string() << std::endl;

	// 名前を変更
	client.editNickname(message.serverID, "VEMT");
	logging::debug << " - Changed name to VEMT. serverID = " << message.serverID.string() << std::endl;

	// BOT管理者権限を作成
	auto bot_admin = client.createRole(message.serverID, "BOT-Admin", SleepyDiscord::NONE, 0xff0000u, true, true).cast();
	logging::debug << " - Created Bot Admin role. serverID = " << message.serverID.string() << std::endl;

	// 出展者権限を作成
	auto exhibitor_role = client.createRole(message.serverID, "Exhibitor", SleepyDiscord::NONE, 0x00ffffu, true, true).cast();
	logging::debug << " - Created Exhibitor role. serverID = " << message.serverID.string() << std::endl;

	// 出展者権限を作成
	auto manager_role = client.createRole(message.serverID, "Manager", SleepyDiscord::NONE, 0x00cc00u, true, true).cast();
	logging::debug << " - Created Manager role. serverID = " << message.serverID.string() << std::endl;

	// カテゴリを作成
	auto category = client.createCategory(message.serverID, "bot").cast();
	logging::debug << " - Created BOT channel. serverID = " << message.serverID.string() << std::endl;

	auto exhibitor_cat = client.createCategory(message.serverID, "contact").cast();
	client.editChannelPermissions(exhibitor_cat, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role");

	logging::debug << " - Created EXHIBITOR channel. server ID = " << message.serverID.string() << std::endl;

	// チャンネルを作成
	auto bot_control_ch = client.createTextChannel(message.serverID, "bot-control", category.ID).cast();
	client.editChannelTopic(bot_control_ch, u8"BOT操作専用チャンネルです。BOT-Adminのみが読み書きできます。`+help`でヘルプを表示します。");
	client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(bot_admin.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, sd::Permission::MANAGE_ROLES, "role");
	client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
	client.editChannelPermissions(bot_control_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role");
	logging::debug << " - Created bot-control channel. serverID = " << message.serverID.string() << std::endl;

	auto entry_ch = client.createTextChannel(message.serverID, "entry", category.ID).cast();
	client.editChannelTopic(entry_ch, u8"出展エントリー用のチャンネルです。エントリー期間中のみ書き込みでき、`+entry`コマンドでエントリーできます。");
	client.editChannelPermissions(entry_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
	client.editChannelPermissions(entry_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " - Created entry channel. serverID = " << message.serverID.string() << std::endl;

	auto query_ch = client.createTextChannel(message.serverID, "query", category.ID).cast();
	client.editChannelTopic(query_ch, u8"BOTへ各種情報の問い合わせ用のチャンネルです。Manager権限の方が利用できます。`+help`でヘルプを表示します。");
	client.editChannelPermissions(query_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
	client.editChannelPermissions(query_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(manager_role.ID.number()),
		sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, 0, "role");
	client.editChannelPermissions(query_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()),
		0, sd::Permission::SEND_MESSAGES | sd::Permission::READ_MESSAGES, "role");
	logging::debug << " - Created query channel. serverID = " << message.serverID.string() << std::endl;

	auto status_ch = client.createTextChannel(message.serverID, "status", category.ID).cast();
	client.editChannelTopic(status_ch, u8"BOTステータス確認用のチャンネルです。ミュート推奨です。");
	client.editChannelPermissions(status_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(vemt_bot_role.ID.number()), sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, 0, "role");
	client.editChannelPermissions(status_ch, SleepyDiscord::Snowflake<SleepyDiscord::Overwrite>(everyone.ID.number()), 0, sd::Permission::SEND_MESSAGES | sd::Permission::MANAGE_CHANNELS | sd::Permission::MANAGE_ROLES, "role");
	logging::debug << " - Created status channel. serverID = " << message.serverID.string() << std::endl;


	// セッティングに格納

	auto _server_id = message.serverID.number();
	auto _bot_cat_id = category.ID.number();
	auto _bot_control_ch_id = bot_control_ch.ID.number();
	auto _entry_ch_id = entry_ch.ID.number();
	auto _status_ch_id = status_ch.ID.number();
	auto _query_ch_id = status_ch.ID.number();
	auto _exhibitor_cat_id = exhibitor_cat.ID.number();
	auto _bot_admin_role_id = bot_admin.ID.number();
	auto _exhibitor_role_id = exhibitor_role.ID.number();
	auto _manager_role_id = manager_role.ID.number();
	auto _vemt_bot_role_id = vemt_bot_role.ID.number();
	auto _everyone_role_id = everyone.ID.number();

	Settings::createSettings(
		_server_id, _bot_cat_id, _bot_control_ch_id, _entry_ch_id, _status_ch_id, _query_ch_id,
		_exhibitor_cat_id, _bot_admin_role_id, _exhibitor_role_id, _manager_role_id, _vemt_bot_role_id, _everyone_role_id);

	// 完了通知
	client.sendSuccessMessage(message.channelID, L"サーバーの初期化が完了しました。");
	logging::info << "Finished initializing the server. serverID = " << message.serverID.string() << std::endl;

}
